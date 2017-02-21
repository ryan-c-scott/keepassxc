if(WITH_XC_SSHKEYS)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

    set(sshkeys_SOURCES
        SshKeys.cpp
    )

    qt5_wrap_ui(sshkeys_SOURCES)
    
    add_library(sshkeys STATIC ${sshkeys_SOURCES})
    target_link_libraries(sshkeys Qt5::Core Qt5::Concurrent Qt5::Widgets Qt5::Network)
    
    add_executable(ssh-ask-pipe WIN32 ssh-ask-pipe.c)
endif()
