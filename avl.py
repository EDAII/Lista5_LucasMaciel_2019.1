class BinaryElem:
    def __init__(self, value):
        self.child_left = None
        self.child_right = None
        self.parent = None
        self.value = value
        self.balancing_factor = 0


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

    def left_rotate(self, elem):
        new_parent = elem.child_right
        new_child = new_parent.child_left
        new_parent.child_left = elem
        new_parent.parent = elem.parent
        elem.child_right = new_child
        elem.parent = new_parent
        return new_parent

    def right_rotate(self, elem):
        new_parent = elem.child_left
        new_child = new_parent.child_right
        new_parent.child_right = elem
        new_parent.parent = elem.parent
        elem.child_left = new_child
        elem.parent = new_parent
        return new_parent

    def balancing(self, child, parent):
        if child.value > parent.value:
            parent.balancing_factor += 1
        else:
            parent.balancing_factor -= 1
        if parent.balancing_factor == 0:
            # se depois de calc o balanc == 0, entao nao vai
            # afetar os nós anteriores
            return
        if parent.balancing_factor <= -2:
            if child.balancing_factor >= 1:
                # se o desbalanceamento estiver para dentro, realiza duas
                # rotacoes (child e parent)
                child = self.left_rotate(child)
            parent = self.right_rotate(parent)
            parent.balancing_factor += 1
            parent.child_right.balancing_factor += 2
        elif parent.balancing_factor >= 2:
            if child.balancing_factor <= -1:
                # se desbalanceamento estiver dentro, realiza duas
                # rotacoes (child e parent)
                child = self.right_rotate(child)
            parent = self.left_rotate(parent)
            parent.balancing_factor -= 1
            parent.child_right.balancing_factor -= 2
        if parent.parent != None:
            self.balancing(parent, parent.parent)
        else:
            # atualiza a raiz
            print("new = ", parent.value)
            self.root = parent

    def insert_elem(self, elem):
        if isinstance(elem, BinaryElem):
            parent = self.search_pos_insert(elem)
            elem.parent = parent
            if parent != None:
                if elem.value > parent.value:
                    parent.child_right = elem
                    self.balancing(elem, parent)
                else:
                    parent.child_left = elem
                    self.balancing(elem, parent)
            else:
                # elem adicionado é a raiz se não houver outros elementos
                self.root = elem

    def in_order(self, elem=0):
        if elem == 0:
            elem = self.root
        if elem != None:
            self.in_order(elem.child_left)
            print(elem.value, " = ", elem.balancing_factor, end=", ")
            self.in_order(elem.child_right)

    def pre_order(self, elem=0):
        if elem == 0:
            elem = self.root
        if elem != None:
            print(elem.value, end=" ")
            self.pre_order(elem.child_left)
            self.pre_order(elem.child_right)


def main():
    tree = BinaryTree()
    array = [20, 33, 5, 9, 3, 2, 56]
    for elem in array:
        tree.insert_elem(BinaryElem(elem))
    print("In Order")
    tree.in_order()
    print()
    tree.pre_order()
    print()


main()
