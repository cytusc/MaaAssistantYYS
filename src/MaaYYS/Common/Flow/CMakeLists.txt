add_library(YYSFlow STATIC
    YYSFlowBase.cpp
    YYSWaitFlow.cpp
    YYSRetryFlow.cpp
    YYSLoopFlow.cpp
    YYSActionFlow.cpp
)

target_include_directories(YYSFlow PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(YYSFlow PUBLIC MaaYYS)
