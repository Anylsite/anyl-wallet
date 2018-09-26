#!/usr/bin/env bash

set -e
set -x
SOLC_URL_LINUX='https://github.com/ethereum/solidity/releases/download/v0.4.23/solc-static-linux'
SOLC_VERSION='v0.4.23'

fail() {
    if [[ $- == *i* ]]; then
       red=`tput setaf 1`
       reset=`tput sgr0`

       echo "${red}==> ${@}${reset}"
    fi
    exit 1
}

info() {
    if [[ $- == *i* ]]; then
        blue=`tput setaf 4`
        reset=`tput sgr0`

        echo "${blue}${@}${reset}"
    fi
}

success() {
    if [[ $- == *i* ]]; then
        green=`tput setaf 2`
        reset=`tput sgr0`
        echo "${green}${@}${reset}"
    fi

}

warn() {
    if [[ $- == *i* ]]; then
        yellow=`tput setaf 3`
        reset=`tput sgr0`

        echo "${yellow}${@}${reset}"
    fi
}


if [[ "${TRAVIS_OS_NAME}" == "osx" ]]; then
    SOLC_URL=${SOLC_URL_MACOS}
else
    SOLC_URL=${SOLC_URL_LINUX}
fi

[ -z "${SOLC_URL}" ] && fail 'missing SOLC_URL'
[ -z "${SOLC_VERSION}" ] && fail 'missing SOLC_VERSION'

if [ ! -x $HOME/.bin/solc-${SOLC_VERSION} ]; then
    curl -L ${SOLC_URL} > /usr/local/bin/solc-${SOLC_VERSION}
    chmod 775 /usr/local/bin/solc-${SOLC_VERSION}
    ln -s /usr/local/bin/solc-${SOLC_VERSION} /usr/local/bin/solc

    success "solc ${SOLC_VERSION} installed"
else
    info 'using cached solc'
fi
