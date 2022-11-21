add_executable(tests tests.c)
target_link_libraries(tests lib)

add_test(NAME shoppinglist
        COMMAND tests shoppinglist)
add_test(NAME distances
        COMMAND tests distances)
add_test(NAME normal_prices
        COMMAND tests normal_prices)
add_test(NAME discounts
        COMMAND tests discounts)
add_test(NAME sum_shoppinglist
        COMMAND tests sum_shoppinglist)
