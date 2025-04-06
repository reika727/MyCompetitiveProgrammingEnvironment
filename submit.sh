#!/bin/bash -e

LANGUAGE_ID_CPP_23_GPP=5028
CONTEST_ID=$1
PROBLEM_ID=$2
SRC=source.cpp

if [ "$3" = '--force' ]; then
        echo -e '\e[31;1mSUBMISSION FORCED!!\e[m'
else
        ./test-samples.sh "$CONTEST_ID" "$PROBLEM_ID"
fi

echo 'submitting source code...'

read -r REVEL_SESSION < .REVEL_SESSION

CSRF_TOKEN=$(
          echo -e "${REVEL_SESSION//\%/\\x}" \
        | grep --text --only-matching --perl-regexp 'csrf_token:\K.+?='
)

revel_session="$REVEL_SESSION"  \
csrf_token="$CSRF_TOKEN" \
curl --variable '%revel_session'                              \
     --variable '%csrf_token'                                 \
     --url "https://atcoder.jp/contests/$CONTEST_ID/submit"   \
     --request POST                                           \
     --expand-cookie 'REVEL_SESSION={{revel_session}}'        \
     --form data.TaskScreenName="${CONTEST_ID}_${PROBLEM_ID}" \
     --form data.LanguageId="$LANGUAGE_ID_CPP_23_GPP"         \
     --form sourceCode="<$SRC"                                \
     --expand-form csrf_token='{{csrf_token}}'

echo 'done.'
