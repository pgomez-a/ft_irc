test_name="REGISTRATION ERRORS"
echo $1 $2
bad_registration()
{
	echo "NICK thisistooooolong"
	sleep 1
	echo "NICK b@dchar"
	sleep 1
	echo "USER fewparams"
	sleep 1
	echo "USER beforenick 0 0 0"
}

echo ":nameless 432   :Erroneous nickname
:nameless 432   :Erroneous nickname
:nameless 461  USER :Not enough parameters" > expected_output

(
	bad_registration | nc -c localhost $1 > actual_output
	sleep 1
	bash test_validation.sh "$test_name"
)

test_name="PASS ERRORS"
bad_pass()
{
	echo "PASS"
	sleep 1
	echo "PASS 🙊"
	sleep 1
	echo "PASS incorrect"
	sleep 1
	echo "PASS pass multiple args" #this is a valid PASS call
	sleep 1
	echo "PASS $2"
	sleep 1
	echo "NICK ni"
	sleep 1
	echo "USER u 0 0 0"
	sleep 1
	echo "PASS $2"
}

echo ":nameless 461  PASS :Not enough parameters
:nameless MODE ni :+
:nameless 001 ni  :Welcome to the Internet Relay Network ni!u@127.0.0.1
:nameless 002 ni  :Your host is nameless, running version 0.1
:nameless 003 ni  :This server was created in 1995
:nameless 462 ni  :Unauthorized command (already registered)" > expected_output

(
	bad_pass $1 $2 | nc -c localhost $1 > actual_output
	sleep 1
	bash test_validation.sh "$test_name"
)

test_name="NICK ERRORS"
bad_registration()
{
	echo "NICK"
	sleep 1
	echo "NICK b@dchar"
	sleep 1
	echo "NICK taken"
	sleep 2
	echo "NICK taken" | nc -c localhost $1
	sleep 1
}

echo ":nameless 431   :No nickname given
:nameless 432   :Erroneous nickname" > expected_output

(
	bad_registration $1 $2 | nc -c localhost $1 > actual_output
	sleep 1
	bash test_validation.sh "$test_name"
)
