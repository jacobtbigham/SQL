@REM g++ -std=gnu++11 -o main_2 ../main_2.cpp ../includes/token/ftokenize.cpp ../includes/token/token.cpp ../includes/token/stokenize.cpp ../includes/token/state_machine_functions.cpp -Igoogletest/googletest/include -pthread -Lgoogletest/build/lib -lgtest
@REM g++ -std=gnu++11 -o basic_test ../_tests/_test_files/basic_test.cpp ../includes/binary_files/file_record.cpp ../includes/binary_files/utilities.cpp ../includes/sql/sql.cpp ../includes/token/token.cpp ../includes/table/table.cpp ../includes/parser/parser.cpp ../includes/token/stokenize.cpp ../includes/token/state_machine_functions.cpp -Igoogletest/googletest/include -pthread -Lgoogletest/build/lib -lgtest
g++ -std=gnu++11 -o main ../main.cpp ../includes/binary_files/file_record.cpp ../includes/token/ftokenize.cpp ../includes/binary_files/utilities.cpp ../includes/sql/sql.cpp ../includes/token/token.cpp ../includes/table/table.cpp ../includes/parser/parser.cpp ../includes/token/stokenize.cpp ../includes/token/state_machine_functions.cpp -Igoogletest/googletest/include -pthread -Lgoogletest/build/lib -lgtest
@REM g++ -std=gnu++11 -o main_3 ../main_3.cpp -Igoogletest/googletest/include -pthread -Lgoogletest/build/lib -lgtest


