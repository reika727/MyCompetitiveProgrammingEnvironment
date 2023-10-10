#!/bin/bash -e

CONTEST_ID=$1
JSON_NAME=$CONTEST_ID.json

if [ -f "$JSON_NAME" ]; then
        echo "$JSON_NAME has already been created."
else
        ./login.sh
        echo "saving to $JSON_NAME..."
        if JSON=$(node atcoder-utils/dist/get-samples login.cookie.json "$CONTEST_ID"); then
                echo "$JSON" > "$JSON_NAME"
                echo 'done.'
        else
                echo "could not create $JSON_NAME."
                exit 1
        fi
fi
