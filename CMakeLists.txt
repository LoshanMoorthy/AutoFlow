cmake_minimum_required(VERSION 3.20)
project(AutoFlow LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(AutoFlow
  src/app/main.cpp

  src/core/SimClock.h
  src/core/SimulationEngine.h
  src/core/SimulationEngine.cpp

  src/model/Item.h
  src/model/Queue.h
  src/model/Station.h
  src/model/Station.cpp
  src/model/Pipeline.h
  src/model/Pipeline.cpp
  src/model/OrderTracker.h
  src/model/OrderTracker.cpp

  src/metrics/Metrics.h
  src/metrics/Metrics.cpp
  src/metrics/CsvReporter.h
  src/metrics/CsvReporter.cpp
  src/metrics/MetricsServer.h
  src/metrics/MetricsServer.cpp

  src/util/Random.h
  src/util/Random.cpp
  src/util/JsonConfig.h
  src/util/JsonConfig.cpp
 "src/model/OrderTracker.h" "src/model/OrderTracker.cpp" "src/scenario/Scenario.h" "src/scenario/Scenario.cpp")

find_package(httplib CONFIG REQUIRED)
target_link_libraries(AutoFlow PRIVATE httplib::httplib)

if (MSVC)
  target_compile_options(AutoFlow PRIVATE /W4 /permissive-)
  target_compile_definitions(AutoFlow PRIVATE _CRT_SECURE_NO_WARNINGS)
else()
  target_compile_options(AutoFlow PRIVATE -Wall -Wextra -Wpedantic)
endif()