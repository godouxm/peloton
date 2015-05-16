/*-------------------------------------------------------------------------
 *
 * multithreaded_tester.h
 * file description
 *
 * Copyright(c) 2015, CMU
 *
 * /n-store/test/multithreaded_tester.h
 *
 *-------------------------------------------------------------------------
 */

#pragma once

#include <vector>
#include <thread>
#include <functional>
#include <iostream>

#include "common/types.h"
#include "common/pretty_printer.h"
#include "common/logger.h"
#include "common/context.h"

namespace nstore {
namespace test {

//===--------------------------------------------------------------------===//
// Test Harness (common routines)
//===--------------------------------------------------------------------===//

#define MAX_THREADS 1024

extern std::atomic<txn_id_t> txn_id_counter;
extern std::atomic<cid_t> cid_counter;

uint64_t GetThreadId();

template<typename... Args>
void LaunchParallelTest(uint64_t num_threads, Args&&... args) {
  std::vector<std::thread> thread_group;

  // Launch a group of threads
  for (uint64_t thread_itr = 0; thread_itr < num_threads; ++thread_itr) {
    thread_group.push_back(std::thread(args...));
  }

  //Join the threads with the main thread
  for (uint64_t thread_itr = 0; thread_itr < num_threads; ++thread_itr) {
    thread_group[thread_itr].join();
  }
}

inline txn_id_t GetTransactionId() {
  return ++txn_id_counter;
}

inline cid_t GetCommitId() {
  return ++cid_counter;
}

inline Context GetContext(){
  const txn_id_t txn_id = GetTransactionId();
  const cid_t commit_id = GetCommitId();
  Context context(txn_id, commit_id);
  return context;
}


} // End test namespace
} // End nstore namespace



