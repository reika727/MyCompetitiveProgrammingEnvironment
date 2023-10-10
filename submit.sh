#!/bin/bash -e

LANGUAGE_ID_CPP_23_GPP=5028
CONTEST_ID=$1
PROBLEM_ID=$2
JSON_NAME=$CONTEST_ID.json
EXE='compro.out'

./download-samples.sh "$CONTEST_ID"

make "$EXE"

wrong_cases=0
while read -r TEST_CASE; do
        INPUT=$(echo "$TEST_CASE" | jq --raw-output '.input')
        EXPECTED_OUTPUT=$(echo "$TEST_CASE" | jq --raw-output '.output')
        OUTPUT=$(echo "$INPUT" | ./"$EXE")
        if [ "$OUTPUT" == "$EXPECTED_OUTPUT" ]; then
                echo -e '\e[32;1mTEST CASE PASSED!\e[m'
        else
                ((++wrong_cases))
                echo ----------------
                echo 'input:'
                echo "$INPUT" | pr --omit-header --indent=2
                echo 'expected output:'
                echo "$EXPECTED_OUTPUT" | pr --omit-header --indent=2
                echo 'output:'
                echo "$OUTPUT" | pr --omit-header --indent=2
                echo ----------------
        fi
done <<< $(jq --compact-output --raw-output ."$PROBLEM_ID"[] "$JSON_NAME")

if [ "$wrong_cases" != 0 ]; then
        echo -e "\e[31;1m$wrong_cases case(s) failed.\e[m"
        exit 1
fi

echo -e '\e[32;1mALL TEST CASE PASSED!\e[m'
echo 'submitting source code...'

COOKIE_STRING=$(
          jq --raw-output '.cookies[]|[.key, .value]|@csv' login.cookie.json \
        | sed 's/"//g; s/,/=/g'                                              \
        | tr '\n' ';'
)

CSRF_TOKEN=$(
          echo "$COOKIE_STRING"                                             \
        | sed 's/%/\\x/g'                                                   \
        | printf '%b' $(cat)                                                \
        | grep --text --extended-regexp --only-matching 'csrf_token:(.+?=)' \
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
