# cs106L-assignment2
Here is what I have done in assignment2 for downloading dependency packages,.

To download dependency packages, just simply run `./setup.sh` , but it can be slow, so I break it down into a few small steps.
* `git clone https://github.com/microsoft/vcpkg.git`
  * can go to the github website and directly download it.
* The next steps is with reference to https://blog.csdn.net/weixin_41364246/article/details/140123907?
  * In windows, replace content: in file `./vcpkg/scripts/bootstrap.sh` (in line 212) `https://github.com/` with 
    `https://gh-proxy.com/github.com/`
  * replace content in file `./vcpkg/scripts/cmake/vcpkg_download_distfile.cmake` 
``` cmake
//第117行
    vcpkg_list(SET params "x-download" "${arg_FILENAME}")
    # foreach(url IN LISTS arg_URLS)
    #     vcpkg_list(APPEND params "--url=${url}")
    # endforeach()
    # 新增一个变量，存储修改后的url集合，用于在控制台中打印
    vcpkg_list(SET arg_URLS_Real)
    foreach(url IN LISTS arg_URLS)
        # 将第三方库的地址更换为国内镜像源地址，这五个只是我目前找到的，如果有更多的需要替换的地址，形如：
        # string(REPLACE <oldUrl> <newUrl> url "${url}")，按照这个格式继续添加即可
        string(REPLACE "http://download.savannah.nongnu.org/releases/gta/" "https://marlam.de/gta/releases/" url "${url}")
		string(REPLACE "https://github.com/" "https://gh-proxy.com/github.com/" url "${url}")
		string(REPLACE "https://ftp.gnu.org/" "https://mirrors.aliyun.com/" url "${url}")
		string(REPLACE "https://raw.githubusercontent.com/" "https://ghfast.top/https://raw.githubusercontent.com/" url "${url}")
		string(REPLACE "http://ftp.gnu.org/pub/gnu/" "https://mirrors.aliyun.com/gnu/" url "${url}")
		string(REPLACE "https://ftp.postgresql.org/pub/" "https://mirrors.tuna.tsinghua.edu.cn/postgresql/" url "${url}")
		string(REPLACE "https://support.hdfgroup.org/ftp/lib-external/szip/2.1.1/src/" "https://distfiles.macports.org/szip/" url "${url}")

        vcpkg_list(APPEND params "--url=${url}")
        # 存储新的第三方库下载地址
        vcpkg_list(APPEND arg_URLS_Real "${url}")
    endforeach()
    if(NOT vcpkg_download_distfile_QUIET)
        # message(STATUS "Downloading ${arg_URLS} -> ${arg_FILENAME}...")
        # 控制台打印信息时，使用实际的下载地址，因为arg_URLS变量无法修改(我不会改，好像是改不了)
        message(STATUS "Downloading ${arg_URLS_Real} -> ${arg_FILENAME}...")
    endif()
```
* Finally, you can run `./setup.sh`, which will be much faster.