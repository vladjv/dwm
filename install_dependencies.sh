#!/bin/sh

install_dependencies() {
    installed_pkg_mgr=""

    # Search for package manager in PATH
    for pkg_mgr in "apt" "yum" "pacman" "pkg_add" "pkg"; do
        if [ -n "$(command -v -p ${pkg_mgr})" ]; then
            installed_pkg_mgr="${pkg_mgr}"
            break
        fi
    done

    case "${installed_pkg_mgr}" in
        "apt")
            INSTALL_CMD="${SU_CMD} ${installed_pkg_mgr} update && ${SU_CMD} ${installed_pkg_mgr} install -f"
            BUILDDEPS="${INSTALL_CMD} cmake python3 python3-dev gcc g++ build-essential libxft-dev libx11-xcb-dev
                        libxcb1-dev libxcb-res0-dev xcompmgr feh"
            ;;
        "yum")
            INSTALL_CMD="${SU_CMD} ${installed_pkg_mgr} -y update && ${SU_CMD} ${installed_pkg_mgr} install"
            BUILDDEPS="${INSTALL_CMD} python3{,-devel,-pip} cmake gcc{,-c++} kernel-devel libxft libxcb xcompmgr feh"
            ;;
        "pacman")
            INSTALL_CMD="${SU_CMD} ${installed_pkg_mgr} -Syyy && ${SU_CMD} ${installed_pkg_mgr} -S"
            BUILDDEPS="${INSTALL_CMD} base-devel gcc cmake libxft python{,-pip} libxcb xcompmgr feh"
            ;;
        "pkg_add")
            INSTALL_CMD="${SU_CMD} syspatch && ${SU_CMD} ${installed_pkg_mgr} -u && ${SU_CMD} ${installed_pkg_mgr}"
            BUILDDEPS="${INSTALL_CMD} vim python3 py-{py,wheel,pip,setuptools} llvm cmake clang-tools-extra
                        moreutils coreutils libxft lixcb xcompmgr feh"
            ;;
        "pkg")
            INSTALL_CMD="${SU_CMD} syspatch && ${SU_CMD} ${installed_pkg_mgr} -u && ${SU_CMD} ${installed_pkg_mgr}"
            BUILDDEPS="${INSTALL_CMD} vim python3 py-{py,wheel,pip,setuptools} llvm git cmake clang-tools-extra
                        moreutils coreutils libxft lixcb xcompmgr feh"
            ;;
        *)
            printf "\033[0;31mCould not found package manager.\033[0m\n"
            exit 2
    esac

    eval ${BUILDDEPS}

    if [ $? -eq 0 ]; then
        printf "\033[0;32mDependencies were installed correctly\033[0m\n"
        sleep 5
    else
        printf "\033[0;31mError: Dependencies were not installed correctly\033[0m\n"
    fi
}


main() {
    printf "\033[0;32mInstalling dependencies for DWM...\033[0m\n"

    for su_mgr in "sudo" "doas" "su"; do
        if [ -n "$(command -v ${su_mgr})" ]; then
            case "${su_mgr}" in
                "sudo")
                    SU_CMD="${su_mgr}"
                    ;;
                "doas")
                    SU_CMD="${su_mgr}"
                    ;;
                "su")
                    SU_CMD="${su_mgr} ${USER} -c"
                    ;;
                *)
                    SU_CMD=""
                    ;;
            esac
        fi

        if [ "${SU_CMD}" != "" ]; then
            break
        fi

    done

    install_dependencies
}

main
