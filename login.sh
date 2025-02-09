#!/bin/bash

if [ -f login.cookie.json ]; then
        echo 'already logged in to AtCoder.'
        exit 0
fi

read -rp 'User Name: ' USER_NAME
read -rsp 'Password: ' PASSWORD
echo
echo 'logging in to AtCoder...'

if ! JSON=$(npx --yes tsx atcoder-utils/login.mts "$USER_NAME" "$PASSWORD"); then
        echo 'login failed.'
        exit 1
fi

DEFAULT_UMASK=$(umask -p)
umask 0077
echo "$JSON" > login.cookie.json
eval "$DEFAULT_UMASK"
echo 'successfully logged in to AtCoder.'
