#!/bin/bash -e

CONTEST_ID=$1
PROBLEM_ID=$2
JSON_NAME=test-cases/$CONTEST_ID.json
EXE=compro.out

./download-samples.sh "$CONTEST_ID"

make "$EXE"

if ! TEST_CASES=$(jq --compact-output --raw-output ."$PROBLEM_ID"[] "$JSON_NAME"); then
        echo "Problem ID '$PROBLEM_ID' does not exist."
        exit 1
fi

wrong_cases=0
while read -r TEST_CASE; do
        INPUT=$(echo "$TEST_CASE" | jq --raw-output '.input')
        EXPECTED_OUTPUT=$(echo "$TEST_CASE" | jq --raw-output '.output')
        OUTPUT=$(echo "$INPUT" | ./"$EXE")

        if [ "$OUTPUT" != "$EXPECTED_OUTPUT" ]; then
                ((++wrong_cases))
                echo ----------------
                echo 'input:'
                echo "$INPUT" | pr --omit-header --indent=2
                echo 'expected output:'
                echo "$EXPECTED_OUTPUT" | pr --omit-header --indent=2
                echo 'output:'
                echo "$OUTPUT" | pr --omit-header --indent=2
                echo ----------------
                continue
        fi

        echo -e '\e[32;1mTEST CASE PASSED!\e[m'
done <<< "$TEST_CASES"

if [ "$wrong_cases" != 0 ]; then
        echo -e "\e[31;1m$wrong_cases case(s) failed.\e[m"
        exit 1
fi

echo -e '\e[32;1mALL TEST CASES PASSED!\e[m'
