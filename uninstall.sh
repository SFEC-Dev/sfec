#!/bin/sh

NAME="sfec"
BIN_DIR="/usr/local/bin"
CONFIG_DIR=$HOME/.config/${NAME}

sudo rm -f "$BIN_DIR/$NAME"
echo ":: Uninstalled $BIN_DIR/$NAME"

rm -rf ${CONFIG_DIR}
echo ":: Uninstalled ${CONFIG_DIR}"

echo ":: ${NAME} uninstalled successfully"
