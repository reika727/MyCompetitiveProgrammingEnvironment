#!/bin/bash -e

CONTEST_ID=$1
JSON_NAME=cases/$CONTEST_ID.json

./login.sh

if [ -f "$JSON_NAME" ]; then
        echo "$JSON_NAME has already been created."
        exit 0
fi

echo "saving to $JSON_NAME..."

if ! JSON=$(npx --yes tsx atcoder-utils/get-samples.ts login.cookie.json "$CONTEST_ID"); then
        echo "could not create $JSON_NAME."
        exit 1
fi

mkdir --parents "$(dirname "$JSON_NAME")"
echo "$JSON" > "$JSON_NAME"
echo 'done.'
