class BinaryElem:
    def __init__(self, value):
        self.child_left = None
        self.child_right = None
        self.dad = None
        self.value = value


class BinaryTree:
    def __init__(self):
        self.root = None

    def search_pos_insert(self, elem):
        if isinstance(elem, BinaryElem):
            root_cp = self.root
            aux = root_cp
            if root_cp != None:
                while (aux != None):
                    root_cp = aux
                    if elem.value > root_cp.value:
                        aux = root_cp.child_right
                    else:
                        aux = root_cp.child_left
                return root_cp
            else:
                return None

    def insert_elem(self, elem):
        if isinstance(elem, BinaryElem):
            dad = self.search_pos_insert(elem)
            elem.dad = dad
            if dad != None:
                if elem.value > dad.value:
                    dad.child_right = elem
                else:
                    dad.child_left = elem
            else:
                self.root = elem


def insert(tree, elem):
    return tree


def main():
    tree = BinaryTree()
    array = [1, 5, 33, 20, 9, 3, 2, 56]
    for elem in array:
        tree.insert_elem(BinaryElem(elem))

    print(tree.root.child_right.value)


main()
