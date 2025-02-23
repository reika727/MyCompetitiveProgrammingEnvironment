#!/bin/bash

if [ -f login.cookie.json ]; then
        echo 'already logged in to AtCoder.'
        exit 0
fi

echo 'logging in to AtCoder...'

if ! JSON=$(npx --yes tsx atcoder-utils/login.mts); then
        echo 'login failed.'
        exit 1
fi

DEFAULT_UMASK=$(umask -p)
umask 0077
echo "$JSON" > login.cookie.json
eval "$DEFAULT_UMASK"
echo 'successfully logged in to AtCoder.'
