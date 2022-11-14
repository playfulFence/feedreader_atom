#!/bin/bash

mkdir test_outputs

echo "----- FEEDREADER ERROR DETECTION TEST -----"

# first test - executing with no arguments

./feedreader >test_outputs/test_noarg.output 2>test_outputs/test_noarg.stderr

if [ $? == 1 ];
then
    diff --brief "test_expects/expected_help_output.txt" "test_outputs/test_noarg.output" > /dev/null
    if [ $? -ne 1 ];
    then 
        echo -e "[1] NO ARGUMENTS..............[PASSED]"
    else
        echo -e "[1] NO ARGUMENTS..............[WARNING](add help output)"
    fi
else
    echo -e "[1] NO ARGUMENTS..............[FAILED]"
fi


# second test - passing both feedfile and URL link

./feedreader -f feed.txt https://www.youtube.com/watch?v=dQw4w9WgXcQ >test_outputs/test_both_sources.output 2>test_outputs/test_both_sources.stderr

if [ $? == 1 ];
then 
    diff --brief "test_expects/expected_help_output.txt" "test_outputs/test_both_sources.output" > /dev/null
    if [ $? -ne 1 ];
    then
        diff --brief "test_expects/expected_test_both_sources.stderr" "test_outputs/test_both_sources.stderr" >/dev/null
        if [ $? -ne 1 ];
        then 
            echo -e "[2] PASS FEEDFILE AND URL.....[PASSED]"
        else
            echo -e "[2] PASS FEEDFILE AND URL.....[WARNING](bad stderr)"
        fi
    else
        echo -e "[2] PASS FEEDFILE AND URL.....[WARNING](bad help output)"
    fi
else
    echo -e "[2] PASS FEEDFILE AND URL.....[FAILED]"
fi

# third test - passing some meaningless stuff

./feedreader https429xcj fksd oprwe >test_outputs/test_unknown_input.output 2>test_outputs/test_unknown_input.stderr

if [ $? == 1 ];
then 
    diff --brief "test_expects/expected_help_output.txt" "test_outputs/test_both_sources.output" > /dev/null
    if [ $? -ne 1 ];
    then
        diff --brief "test_expects/expected_test_unknown_input.stderr" "test_outputs/test_unknown_input.stderr" >/dev/null
        if [ $? -ne 1 ];
        then 
            echo -e "[3] UNKNOWN INPUT.............[PASSED]"
        else
            echo -e "[3] UNKNOWN INPUT.............[WARNING](bad stderr)"
        fi
    else
        echo -e "[3] UNKNOWN INPUT.............[WARNING](bad help output)"
    fi
else
    echo -e "[3] UNKNOWN INPUT.............[FAILED]"
fi

# fourth test - bad flag

./feedreader -f feed.txt -u -i >test_outputs/test_bad_flag.output 2>test_outputs/test_bad_flag.stderr

if [ $? == 1 ];
then 
    diff --brief "test_expects/expected_help_output.txt" "test_outputs/test_bad_flag.output" > /dev/null
    if [ $? -ne 1 ]; #no stderr output checking since it changes according to the name of bad flag
    then
        echo -e "[4] UNKNOWN FLAG..............[PASSED]"   
    else
        echo -e "[4] UNKNOWN FLAG..............[WARNING](bad help output)"
    fi
else
    echo -e "[4] UNKNOWN FLAG..............[FAILED]"
fi

#fifth test - same extra argument

./feedreader -f feed.txt -u -u >test_outputs/test_more_than_one_u.output 2>test_outputs/test_more_than_one_u.stderr

if [ $? == 1 ];
then 
    diff --brief "test_expects/expected_test_more_than_one_u.stderr" "test_outputs/test_more_than_one_u.stderr" >/dev/null
    if [ $? -ne 1 ];
    then
        echo -e "[5] MULTIPLE -u FLAG..........[PASSED]"
    else
            echo -e "[5] MULTIPLE -u FLAG..........[WARNING](bad stderr)"
    fi
else
    echo -e "[5] MULTIPLE -u FLAG..........[FAILED]"
fi

#sixth test - more than one -a

./feedreader -f feed.txt -u -a -a >test_outputs/test_more_than_one_a.output 2>test_outputs/test_more_than_one_a.stderr

if [ $? == 1 ];
then 
    diff --brief "test_expects/expected_test_more_than_one_a.stderr" "test_outputs/test_more_than_one_a.stderr" >/dev/null
    if [ $? -ne 1 ];
    then
        echo -e "[6] MULTIPLE -a FLAG..........[PASSED]"
    else
        echo -e "[6] MULTIPLE -a FLAG..........[WARNING](bad stderr)"
    fi
else
    echo -e "[6] MULTIPLE -a FLAG..........[FAILED]"
fi


#seventh test - more than one -T

./feedreader -f feed.txt -u -a -T -T >test_outputs/test_more_than_one_T.output 2>test_outputs/test_more_than_one_T.stderr

if [ $? == 1 ];
then 
    diff --brief "test_expects/expected_test_more_than_one_T.stderr" "test_outputs/test_more_than_one_T.stderr" >/dev/null
    if [ $? -ne 1 ];
    then
        echo -e "[7] MULTIPLE -T FLAG..........[PASSED]"
    else
        echo -e "[7] MULTIPLE -T FLAG..........[WARNING](bad stderr)"
    fi
else
    echo -e "[7] MULTIPLE -T FLAG..........[FAILED]"
fi

#eighth - more than one -c flag

./feedreader -f feed.txt -u -a -T -c /path/to/cert -c /path/to/twice >test_outputs/test_more_than_one_c.output 2>test_outputs/test_more_than_one_c.stderr

if [ $? == 1 ];
then 
    diff --brief "test_expects/expected_test_more_than_one_c.stderr" "test_outputs/test_more_than_one_c.stderr" >/dev/null
    if [ $? -ne 1 ];
    then
        echo -e "[8] MULTIPLE -c FLAG..........[PASSED]"
    else
        echo -e "[8] MULTIPLE -c FLAG..........[WARNING](bad stderr)"
    fi
else
    echo -e "[8] MULTIPLE -c FLAG..........[FAILED]"
fi


./feedreader -f feed.txt -u -a -T -C /path/to/certs -C /path/to/twice >test_outputs/test_more_than_one_certaddr.output 2>test_outputs/test_more_than_one_certaddr.stderr

if [ $? == 1 ];
then 
    diff --brief "test_expects/expected_test_more_than_one_certaddr.stderr" "test_outputs/test_more_than_one_certaddr.stderr" >/dev/null
    if [ $? -ne 1 ];
    then
        echo -e "[9] MULTIPLE -C FLAG..........[PASSED]"
    else
        echo -e "[9] MULTIPLE -C FLAG..........[WARNING](bad stderr)"
    fi
else
    echo -e "[9] MULTIPLE -C FLAG..........[FAILED]"
fi


./feedreader -f /this/is/really/wrong/path.txt >test_outputs/test_bad_feedfile.output 2>test_outputs/test_bad_feedfile.stderr

if [ $? == 2 ];
then 
    echo -e "[10] WRONG FEEDFILE PATH......[PASSED]"
else
    echo -e "[10] WRONG FEEDFILE PATH......[FAILED]"
fi




