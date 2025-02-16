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

./login.sh

echo 'submitting source code...'

COOKIE_STRING=$(jq --raw-output '.cookies|map("\(.key)=\(.value)")|join(";")' login.cookie.json)

# jq には将来的に @urid が実装されるかもしれないのでそのときはもう少し短く書けそう
# https://github.com/jqlang/jq/pull/3161
CSRF_TOKEN=$(
        jq --raw-output                                                                                                                   \
        '.cookies|map(select(.key=="REVEL_SESSION"))|first|.value|match("csrf_token%3A(.+?)%00")|.captures|first|.string|gsub("%";"\\x")' \
        login.cookie.json                                                                                                                 \
        | echo -e "$(cat)"
)

curl "https://atcoder.jp/contests/$CONTEST_ID/submit"         \
     --request POST                                           \
     --cookie "$COOKIE_STRING"                                \
     --form data.TaskScreenName="${CONTEST_ID}_${PROBLEM_ID}" \
     --form data.LanguageId="$LANGUAGE_ID_CPP_23_GPP"         \
     --form sourceCode="<$SRC"                                \
     --form csrf_token="$CSRF_TOKEN"

echo 'done.'
