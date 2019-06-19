#!usr/bin/python3

from lang import *
import mysql


class Pro:
    list = []
    answer = " 不好意思，小安暂时回答不了你这个问题哦。"

    def __init__(self):
        pass

    def find_it(self, it, ch):
        index = 1
        for key in it:
            if ch == key:
                self.list.append(index)
                return it[key]
            index = index + 1
        return None

    def find(self, text):
        self.list.clear()
        length = len(text)
        it = root
        for index in range(length):
            if it == 'end':
                break
            it = self.find_it(it, text[index])

        if len(self.list) == 0:
            return self.answer

        sql = "SELECT answer FROM root"
        for u in range(0, len(self.list) - 1):
            sql = sql + "_" + str(self.list[u])
        sql = sql + " WHERE q_id = " + str(self.list[len(self.list) - 1]) + ";"
        print(sql)

        conn = mysql.connect()
        if conn is None:
            return self.answer

        cursor = conn.cursor()
        cursor.execute(sql)
        result = cursor.fetchone()

        if result is None:
            return self.answer

        mysql.close(conn)
        return result[0]
