add_executable(Space_fighter
    game.h
    game.c 
    main.c
)   

target_link_libraries(Space_fighter PRIVATE 
    raylib
)