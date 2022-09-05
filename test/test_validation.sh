#TEST_VALIDATION
name=$1

tr -d '\r' < actual_output > tmp
cat tmp > actual_output
rm tmp

echo -n "$name >> "
if [[ $(echo $(diff --suppress-common-lines actual_output expected_output | wc -l)) != "0"  ]]; then
	echo -e "\033[91m[KO]:\n\n $( diff -y --suppress-common-lines actual_output expected_output)\033[0m"
else
	echo -e "\033[92m[OK]\033[0m"
fi
