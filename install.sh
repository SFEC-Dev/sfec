#!/bin/sh


NAME="sfec"
RELEASE_NAME="sfec-Linux.tar.gz"
RELEASE_URL="https://github.com/SFEC-Dev/sfec/releases/latest/download/${RELEASE_NAME}"
TEMP_DIR=$(mktemp -d)
BIN_DIR="/usr/local/bin"
CONFIG_DIR=$HOME/.config/${NAME}/

export CLR_RESET='\033[1;0m'
export STL_BOLD='\033[1;1m'
export CLR_RED='\033[0;31m'
export CLR_GREEN='\033[0;32m'
export CLR_BLUE='\033[0;34m'

msg() {
  printf "${CLR_BLUE}${STL_BOLD}::${CLR_RESET} ${STL_BOLD}%s${CLR_RESET}\n" "$1"
  shift
  for i in "$@"; do
    printf " ${CLR_BLUE}${STL_BOLD}|${CLR_RESET} ${STL_BOLD}%s${CLR_RESET}\n" "$i"
  done
}

compl() {
  printf "${CLR_GREEN}${STL_BOLD}>>>${CLR_RESET} ${STL_BOLD}%s${CLR_RESET}\n" "$1"
  shift
}

download() {
    compl "Downloading ${RELEASE_NAME}"
    curl -LO --output-dir ${TEMP_DIR} ${RELEASE_URL}
}

unpack() {
    msg "Extract ${RELEASE_NAME}"
    tar -xzf ${TEMP_DIR}/${RELEASE_NAME} -C "$TEMP_DIR/"
}

move() {
    msg "Moving ${NAME} into ${BIN_DIR}"
    SFEC_BIN="$NAME-$VERSION-Linux/bin/sfec"
    sudo mv "$TEMP_DIR/$SFEC_BIN" ${BIN_DIR}
    rm -rf ${TEMP_DIR}
}

create_dir() {
    msg "Created dir ${CONFIG_DIR}"
    mkdir -p ${CONFIG_DIR}
}

main() {
    set -e
    
    download
    
    unpack
    
    VERSION=$(ls ${TEMP_DIR} | head -n 1 | sed 's/[^0-9.]//g')
    msg 'Release version: '${NAME}-${VERSION}
    
    move
    
    create_dir
    
    compl "${NAME} installed successfully"
}

main
