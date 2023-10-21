include(FetchContent)
FetchContent_Declare(ZStrGitRepo
  URL "https://github.com/mateidavid/zstr/archive/refs/tags/v1.0.7.tar.gz"
)
FetchContent_MakeAvailable(ZStrGitRepo) # defines INTERFACE target 'zstr::zstr'