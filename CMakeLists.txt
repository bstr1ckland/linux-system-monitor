find_package(ftxui REQUIRED)
add_executable(main main.cpp)
target_link_libraries(main
  ftxui::screen
  ftxui::dom
  ftxui::component
)