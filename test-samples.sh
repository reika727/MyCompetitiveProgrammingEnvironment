#!/bin/bash -e

CONTEST_ID=$1
PROBLEM_ID=$2
JSON_NAME=$CONTEST_ID.json
EXE='compro.out'

./download-samples.sh "$CONTEST_ID"

make "$EXE"

wrong_cases=0
if TEST_CASES=$(jq --compact-output --raw-output ."$PROBLEM_ID"[] "$JSON_NAME"); then
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
        done <<< "$TEST_CASES"
else
        echo "Problem ID '$PROBLEM_ID' does not exist."
        exit 1
fi

if [ "$wrong_cases" != 0 ]; then
        echo -e "\e[31;1m$wrong_cases case(s) failed.\e[m"
        exit 1
fi

echo -e '\e[32;1mALL TEST CASE PASSED!\e[m'
