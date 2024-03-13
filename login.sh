#!/bin/bash

if [ -f login.cookie.json ]; then
        echo 'already loged in to AtCoder.'
        exit 0
fi

read -rp 'User Name: ' USER_NAME
read -rsp 'Password: ' PASSWORD
echo
echo 'logging in to AtCoder...'

if ! JSON=$(node atcoder-utils/dist/login "$USER_NAME" "$PASSWORD"); then
        echo 'login failed.'
        exit 1
fi

DEFAULT_UMASK=$(umask -p)
umask 0077
echo "$JSON" > login.cookie.json
eval "$DEFAULT_UMASK"
echo 'successfully loged in to AtCoder.'
