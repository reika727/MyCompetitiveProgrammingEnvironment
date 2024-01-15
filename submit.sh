#!/bin/bash -e

LANGUAGE_ID_CPP_23_GPP=5028
CONTEST_ID=$1
PROBLEM_ID=$2

./test-samples.sh "$CONTEST_ID" "$PROBLEM_ID"

echo 'submitting source code...'

COOKIE_STRING=$(
          jq --raw-output '.cookies[]|[.key, .value]|@csv' login.cookie.json \
        | sed 's/"//g; s/,/=/g'                                              \
        | tr '\n' ';'
)

CSRF_TOKEN=$(
          echo "$COOKIE_STRING"                                           \
        | sed 's/%/\\x/g'                                                 \
        | printf '%b' "$(cat)"                                            \
        | grep --text --extended-regexp --only-matching 'csrf_token:.+?=' \
        | sed 's/csrf_token://'
)

curl "https://atcoder.jp/contests/$CONTEST_ID/submit"         \
     --cookie "$COOKIE_STRING"                                \
     --form data.TaskScreenName="${CONTEST_ID}_${PROBLEM_ID}" \
     --form data.LanguageId="$LANGUAGE_ID_CPP_23_GPP"         \
     --form sourceCode='<source.cpp'                          \
     --form csrf_token="$CSRF_TOKEN"                          \
     --silent

echo 'done.'
