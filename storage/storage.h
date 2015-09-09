#pragma once

#include <string>
#include <memory>
#include <thread>
#include <mutex>
#include "page.h"
#include "cache.h"
#include "asyncwriter.h"

namespace storage {

const uint64_t defaultPageSize = 10 * 1024 * 1024; // 10Mb
const size_t defaultcacheSize = 10000;
const size_t defaultcachePoolSize = 100;

class DataStorage {
public:
  typedef std::shared_ptr<DataStorage> PDataStorage;

public:
  static PDataStorage Create(const std::string &ds_path,
                             uint64_t page_size = defaultPageSize);
  static PDataStorage Open(const std::string &ds_path);
  ~DataStorage();
  void Close();
  bool havePage2Write() const;
  append_result append(const Meas& m);
  append_result append(const Meas::PMeas begin, const size_t meas_count);

  Meas::MeasList readInterval(Time from, Time to);
  Meas::MeasList readInterval(const IdArray &ids, storage::Flag source, storage::Flag flag, Time from, Time to);
  Meas::MeasList curValues(const IdArray&ids);
  Page::PPage getCurPage();
  void createNewPage();

  Time pastTime() const;
  void setPastTime(const Time &t);

  void enableCacheDynamicSize(bool flg);
  bool cacheDynamicSize() const;

  size_t getPoolSize()const;
  void setPoolSize(size_t sz);
  size_t getCacheSize()const;
  void setCacheSize(size_t sz);

  void loadCurValues(const IdArray&ids);
private:
  DataStorage();
  std::list<std::string> pageList() const;
  void writeCache();
protected:
  std::string m_path;
  Page::PPage m_curpage;
  uint64_t m_default_page_size;
  std::mutex m_write_mutex;

  storage::Cache::PCache m_cache;

  AsyncWriter m_cache_writer;
  CachePool m_cache_pool;
  CurValuesCache m_cur_values;
  Time m_past_time;
  friend class storage::Cache;
};
}
