//===----------------------------------------------------------------------===//
//
//                         Peloton
//
// aggregate_sql_test.cpp
//
// Identification: test/sql/aggregate_sql_test.cpp
//
// Copyright (c) 2015-16, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#include <memory>

#include "catalog/catalog.h"
#include "common/harness.h"
#include "executor/create_executor.h"
#include "planner/create_plan.h"

#include "sql/sql_tests_util.h"

namespace peloton {
namespace test {

class AggregateSQLTests : public PelotonTest {};

TEST_F(AggregateSQLTests, MinMaxTest) {
  catalog::Catalog::GetInstance()->CreateDatabase(DEFAULT_DB_NAME, nullptr);

  std::vector<ResultType> result;

  // Create a table first
  // TODO: LM: I didn't test boolean here because we can't insert booleans into
  // the table
  SQLTestsUtil::ExecuteSQLQuery(
      "CREATE TABLE test(a INT PRIMARY KEY, b SMALLINT, c "
      "INT, d BIGINT, e DECIMAL, f DOUBLE, g VARCHAR, h TIMESTAMP);",
      result);

  // Insert tuples into table
  SQLTestsUtil::ExecuteSQLQuery(
      "INSERT INTO test VALUES (2, 2, 2, 2, 2.0, "
      "2.0, '23', '2016-12-06 00:00:02-04');",
      result);
  SQLTestsUtil::ExecuteSQLQuery(
      "INSERT INTO test VALUES (1, 1, 1, 1, 1.0, "
      "1.0, '15', '2016-12-06 00:00:01-04');",
      result);
  SQLTestsUtil::ExecuteSQLQuery(
      "INSERT INTO test VALUES (4, 4, 4, 4, 4.0, "
      "4.0, '41', '2016-12-06 00:00:04-04');",
      result);
  SQLTestsUtil::ExecuteSQLQuery(
      "INSERT INTO test VALUES (3, 3, 3, 3, 3.0, "
      "3.0, '33', '2016-12-06 00:00:03-04');",
      result);

  SQLTestsUtil::ShowTable(DEFAULT_DB_NAME, "test");

  // test small int
  SQLTestsUtil::ExecuteSQLQuery("SELECT min(b) from test", result);
  EXPECT_EQ(result[0].second[0], '1');
  SQLTestsUtil::ExecuteSQLQuery("SELECT max(b) from test", result);
  EXPECT_EQ(result[0].second[0], '4');

  // test int
  SQLTestsUtil::ExecuteSQLQuery("SELECT min(a) from test", result);
  EXPECT_EQ(result[0].second[0], '1');
  SQLTestsUtil::ExecuteSQLQuery("SELECT max(a) from test", result);
  EXPECT_EQ(result[0].second[0], '4');

  // test big int
  SQLTestsUtil::ExecuteSQLQuery("SELECT min(d) from test", result);
  EXPECT_EQ(result[0].second[0], '1');
  SQLTestsUtil::ExecuteSQLQuery("SELECT max(d) from test", result);
  EXPECT_EQ(result[0].second[0], '4');

  // test double
  SQLTestsUtil::ExecuteSQLQuery("SELECT min(e) from test", result);
  EXPECT_EQ(result[0].second[0], '1');
  SQLTestsUtil::ExecuteSQLQuery("SELECT max(e) from test", result);
  EXPECT_EQ(result[0].second[0], '4');

  // test decimal
  SQLTestsUtil::ExecuteSQLQuery("SELECT min(f) from test", result);
  EXPECT_EQ(result[0].second[0], '1');
  SQLTestsUtil::ExecuteSQLQuery("SELECT max(f) from test", result);
  EXPECT_EQ(result[0].second[0], '4');

  /*
   * TODO: LM: I commented these out because we will core dump when doing
             min/max on varchar/timestamp
  // test varchar
  SQLTestsUtil::ExecuteSQLQuery("SELECT", "SELECT min(g) from test", result);
  EXPECT_EQ(result[0].second[0], '1');
  SQLTestsUtil::ExecuteSQLQuery("SELECT", "SELECT max(g) from test", result);
  EXPECT_EQ(result[0].second[0], '4');

  // test timestamp
  SQLTestsUtil::ExecuteSQLQuery("SELECT", "SELECT min(h) from test", result);
  EXPECT_EQ(result[0].second[18], '1');
  SQLTestsUtil::ExecuteSQLQuery("SELECT", "SELECT max(h) from test", result);
  EXPECT_EQ(result[0].second[18], '4');
  */

  // free the database just created
  auto &txn_manager = concurrency::TransactionManagerFactory::GetInstance();
  auto txn = txn_manager.BeginTransaction();
  catalog::Catalog::GetInstance()->DropDatabaseWithName(DEFAULT_DB_NAME, txn);
  txn_manager.CommitTransaction(txn);
}

}  // namespace test
}  // namespace peloton
