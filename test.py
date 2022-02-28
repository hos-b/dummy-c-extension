import dummy


test_obj = dummy.Dummy(5)
test_obj.seed = 10

test_obj.randi_min = -10
test_obj.randi_max = +10

rand_mat = test_obj.irand2d(50, 50)
print(rand_mat)