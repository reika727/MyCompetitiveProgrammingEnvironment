#!/bin/bash -e

LANGUAGE_ID_CPP_23_GPP=5028
CONTEST_ID=$1
PROBLEM_ID=$2

if [[ "$3" == "--force" ]]; then
        echo -e "\e[31;1mSUBMISSION FORCED!!\e[m"
else
        ./test-samples.sh "$CONTEST_ID" "$PROBLEM_ID"
fi

echo 'submitting source code...'

COOKIE_STRING=$(jq --raw-output '.cookies|map("\(.key)=\(.value)")|join(";")' login.cookie.json)

CSRF_TOKEN=$(
          echo -e "${COOKIE_STRING//%/\\x}"                   \
        | sed --regexp-extended 's/.*csrf_token:(.+?=).*/\1/'
)

curl "https://atcoder.jp/contests/$CONTEST_ID/submit"         \
     --cookie "$COOKIE_STRING"                                \
     --form data.TaskScreenName="${CONTEST_ID}_${PROBLEM_ID}" \
     --form data.LanguageId="$LANGUAGE_ID_CPP_23_GPP"         \
     --form sourceCode='<source.cpp'                          \
     --form csrf_token="$CSRF_TOKEN"                          \
     --silent

echo 'done.'
