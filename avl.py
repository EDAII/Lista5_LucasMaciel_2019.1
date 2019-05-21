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
        elem.child_right = new_child
        # diminuir fator
        elem.balancing_factor -= 1
        new_parent.balancing_factor -= 1
        return new_parent

    def right_rotate(self, elem):
        new_parent = elem.child_left
        new_child = new_parent.child_right
        new_parent.child_right = elem
        elem.child_left = new_child
        # diminuir fator
        elem.balancing_factor += 1
        new_parent.balancing_factor += 1
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
        elif parent.balancing_factor <= -2:
            print("valor = ", parent.value)
            if child.balancing_factor >= 1:
                # se o desbalanceamento estiver para dentro, realiza duas
                # rotacoes (child e parent)
                child = self.left_rotate(child)
            parent = self.right_rotate(parent)
            return
        elif parent.balancing_factor >= 2:
            if child.balancing_factor <= -1:
                # se desbalanceamento estiver dentro, realiza duas
                # rotacoes (child e parent)
                child = self.right_rotate(child)
            parent = self.left_rotate(parent)
            return
        elif parent.parent != None:
            return self.balancing(parent, parent.parent)

    def insert_elem(self, elem):
        if isinstance(elem, BinaryElem):
            parent = self.search_pos_insert(elem)
            elem.parent = parent
            if parent != None:
                if elem.value > parent.value:
                    parent.child_right = elem
                    # parent = self.balancing(elem, parent)
                else:
                    parent.child_left = elem
                    parent = self.balancing(elem, parent)
            else:
                # elem adicionado é a raiz se não houver outros elementos
                self.root = elem

    def in_order(self, elem=0):
        if elem == 0:
            elem = self.root
        if elem != None:
            self.in_order(elem.child_left)
            print(elem.value, " h = ", elem.balancing_factor, end=", ")
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
    array = [20, 33, 5, 9, 3, 2]  # , 56]
    for elem in array:
        tree.insert_elem(BinaryElem(elem))
    print("In Order")
    tree.in_order()
    print()
    tree.pre_order()
    print()


main()
