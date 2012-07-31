/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package org.apache.hadoop.hive.ql.index.spatial;

import java.util.LinkedHashMap;
import java.util.List;
import java.util.Set;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.apache.hadoop.hive.conf.HiveConf;
import org.apache.hadoop.hive.metastore.api.FieldSchema;
import org.apache.hadoop.hive.metastore.api.Index;
import org.apache.hadoop.hive.metastore.api.StorageDescriptor;
import org.apache.hadoop.hive.metastore.api.Table;
import org.apache.hadoop.hive.ql.Driver;
import org.apache.hadoop.hive.ql.exec.Task;
import org.apache.hadoop.hive.ql.hooks.ReadEntity;
import org.apache.hadoop.hive.ql.hooks.WriteEntity;
import org.apache.hadoop.hive.ql.index.HiveIndexQueryContext;
import org.apache.hadoop.hive.ql.index.TableBasedIndexHandler;
import org.apache.hadoop.hive.ql.metadata.HiveException;
import org.apache.hadoop.hive.ql.metadata.HiveUtils;
import org.apache.hadoop.hive.ql.metadata.VirtualColumn;
import org.apache.hadoop.hive.ql.optimizer.IndexUtils;
import org.apache.hadoop.hive.ql.parse.ParseContext;
import org.apache.hadoop.hive.ql.plan.ExprNodeDesc;
import org.apache.hadoop.hive.ql.plan.PartitionDesc;

public class SpatialIndexHandler extends TableBasedIndexHandler {

  private static final Log LOG = LogFactory.getLog(SpatialIndexHandler.class.getName());


  @Override
  public void analyzeIndexDefinition(Table baseTable, Index index,
      Table indexTable) throws HiveException {
    StorageDescriptor storageDesc = index.getSd();
    if (this.usesIndexTable() && indexTable != null) {
      StorageDescriptor indexTableSd = storageDesc.deepCopy();
      List<FieldSchema> indexTblCols = indexTableSd.getCols();
      FieldSchema bucketFileName = new FieldSchema("_bucketname", "string", "");
      indexTblCols.add(bucketFileName);
      FieldSchema offSets = new FieldSchema("_offsets", "array<bigint>", "");
      indexTblCols.add(offSets);
      indexTable.setSd(indexTableSd);
    }
  }

  @Override
  protected Task<?> getIndexBuilderMapRedTask(Set<ReadEntity> inputs, Set<WriteEntity> outputs,
      List<FieldSchema> indexField, boolean partitioned,
      PartitionDesc indexTblPartDesc, String indexTableName,
      PartitionDesc baseTablePartDesc, String baseTableName, String dbName) throws HiveException {

    String indexCols = HiveUtils.getUnparsedColumnNamesFromFieldSchema(indexField);

    //form a new insert overwrite query.
    StringBuilder command= new StringBuilder();
    LinkedHashMap<String, String> partSpec = indexTblPartDesc.getPartSpec();

    command.append("INSERT OVERWRITE TABLE " + HiveUtils.unparseIdentifier(indexTableName ));
    if (partitioned && indexTblPartDesc != null) {
      command.append(" PARTITION ( ");
      List<String> ret = getPartKVPairStringArray(partSpec);
      for (int i = 0; i < ret.size(); i++) {
        String partKV = ret.get(i);
        command.append(partKV);
        if (i < ret.size() - 1) {
          command.append(",");
        }
      }
      command.append(" ) ");
    }

    command.append(" SELECT ");
    command.append("MBB ( " + indexCols + " )");
    command.append(",");

    command.append(VirtualColumn.FILENAME.getName());
    command.append(",");
    command.append(" collect_set (");
    command.append(VirtualColumn.BLOCKOFFSET.getName());
    command.append(") ");
    command.append(" FROM " + HiveUtils.unparseIdentifier(baseTableName));
    LinkedHashMap<String, String> basePartSpec = baseTablePartDesc.getPartSpec();
    if(basePartSpec != null) {
      command.append(" WHERE ");
      List<String> pkv = getPartKVPairStringArray(basePartSpec);
      for (int i = 0; i < pkv.size(); i++) {
        String partKV = pkv.get(i);
        command.append(partKV);
        if (i < pkv.size() - 1) {
          command.append(" AND ");
        }
      }
    }
    command.append(" GROUP BY ");
    command.append(indexCols + ", " + VirtualColumn.FILENAME.getName() );

    HiveConf builderConf = new HiveConf(getConf(), SpatialIndexHandler.class);
    builderConf.setBoolVar(HiveConf.ConfVars.HIVEMERGEMAPFILES, false);
    builderConf.setBoolVar(HiveConf.ConfVars.HIVEMERGEMAPREDFILES, false);
    Task<?> rootTask = IndexUtils.createRootTask(builderConf, inputs, outputs,
        command, partSpec, indexTableName, dbName);
    return rootTask;
  }

  @Override
  public void generateIndexQuery(List<Index> indexes, ExprNodeDesc predicate,
    ParseContext pctx, HiveIndexQueryContext queryContext) {

    Index index = indexes.get(0);
    // pass residual predicate back out for further processing
    // currently we assume that there are no residual predicate
    queryContext.setResidualPredicate(null);
    // setup TableScanOperator to change input format for original query
    queryContext.setIndexInputFormat(HiveSpatialIndexInputFormat.class.getName());

    // Build reentrant QL for index query
    StringBuilder qlCommand = new StringBuilder("INSERT OVERWRITE DIRECTORY ");

    String tmpFile = pctx.getContext().getMRTmpFileURI();
    queryContext.setIndexIntermediateFile(tmpFile);
    qlCommand.append( "\"" + tmpFile + "\" ");            // QL includes " around file name
    qlCommand.append("SELECT `_bucketname` ,  `_offsets` FROM ");
    qlCommand.append(HiveUtils.unparseIdentifier(index.getIndexTableName()));
    qlCommand.append(" WHERE ");

    // currently we only support single spatial predicate
    String predicateString = predicate.getExprString();
    qlCommand.append(predicateString);

    // generate tasks from index query string
    LOG.info("Generating tasks for re-entrant QL query: " + qlCommand.toString());
    HiveConf queryConf = new HiveConf(pctx.getConf(), SpatialIndexHandler.class);
    HiveConf.setBoolVar(queryConf, HiveConf.ConfVars.COMPRESSRESULT, false);
    Driver driver = new Driver(queryConf);
    driver.compile(qlCommand.toString(), false);
    queryContext.addAdditionalSemanticInputs(driver.getPlan().getInputs());
    queryContext.setQueryTasks(driver.getPlan().getRootTasks());
    return;
  }



  @Override
  public boolean checkQuerySize(long querySize, HiveConf hiveConf) {
    long minSize = hiveConf.getLongVar(HiveConf.ConfVars.HIVEOPTINDEXFILTER_COMPACT_MINSIZE);
    long maxSize = hiveConf.getLongVar(HiveConf.ConfVars.HIVEOPTINDEXFILTER_COMPACT_MAXSIZE);
    if (maxSize < 0) {
      maxSize = Long.MAX_VALUE;
    }
    return (querySize > minSize & querySize < maxSize);
  }

  @Override
  public boolean usesIndexTable() {
    return true;
  }

}
