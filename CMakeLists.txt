cmake_minimum_required(VERSION 3.20)
project(AutoFlow LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(AutoFlow
  src/app/main.cpp

  src/core/SimulationEngine.h
  src/core/SimulationEngine.cpp
  src/core/SimClock.h

  src/model/Item.h
  src/model/Queue.h
  src/model/Station.h
  src/model/Station.cpp
  src/model/Pipeline.h
  src/model/Pipeline.cpp

  src/metrics/Metrics.h
  src/metrics/Metrics.cpp
  src/metrics/CsvReporter.h
  src/metrics/CsvReporter.cpp

  src/util/Random.h
  src/util/Random.cpp
  src/util/JsonConfig.h
  src/util/JsonConfig.cpp
)

target_compile_definitions(AutoFlow PRIVATE _CRT_SECURE_NO_WARNINGS)

if (MSVC)
  target_compile_options(AutoFlow PRIVATE /W4 /permissive-)
else()
  target_compile_options(AutoFlow PRIVATE -Wall -Wextra -Wpedantic)
endif()
