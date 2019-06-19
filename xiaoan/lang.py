#!usr/bin/python3
# code=utf-8

root_1_1_1_1 = {'啊': 'end'}

root_1_2_1_1 = {'啊': 'end'}

root_1_1_1 = {'亮': root_1_1_1_1}

root_1_2_1 = {'嘛': root_1_2_1_1}

root_1_1 = {'漂': root_1_1_1,
            '吗': 'end'}

root_1_2 = {'干': root_1_2_1}

root_1 = {'好': root_1_1,
          '在': root_1_2}

root = {'你': root_1}