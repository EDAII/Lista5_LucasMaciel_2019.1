class BinaryElem:
    def __init__(self, value):
        self.left_child = None
        self.right_child = None
        self.parent = None
        self.value = value
        self.right_height = 0
        self.left_height = 0


class BinaryTree:
    def __init__(self):
        self.root = None

    def search_pos_insert(self, elem):
        if isinstance(elem, BinaryElem):
            parent = self.root
            aux = parent
            if parent != None:
                while (aux != None):
                    parent = aux
                    if elem.value > parent.value:
                        aux = parent.right_child
                    else:
                        aux = parent.left_child
                return parent
            else:
                return None

    def fix_height(self, child, parent):
        # Consertar alturas do filho e pai
        l_h = child.left_child.left_height if child.left_child != None else 0
        r_h = child.left_child.right_height if child.left_child != None else 0
        # somando 1 a maior altura dos filhos depois da rotacao
        child.left_height = 1 + (l_h if l_h >= r_h else r_h)
        l_h = child.right_child.left_height if child.right_child != None else 0
        r_h = child.right_child.right_height if child.right_child != None else 0
        child.right_height = 1 + (l_h if l_h >= r_h else r_h)

        l_h = parent.left_child.left_height if parent.left_child != None else 0
        r_h = parent.left_child.right_height if parent.left_child != None else 0
        parent.left_height = 1 + (l_h if l_h >= r_h else r_h)
        l_h = parent.right_child.left_height if parent.right_child != None else 0
        r_h = parent.right_child.right_height if parent.right_child != None else 0
        parent.right_height = 1 + (l_h if l_h >= r_h else r_h)

    def left_rotate(self, elem):
        grand_parent = elem.parent
        print("grand = ", grand_parent.value)
        new_parent = elem.right_child
        new_child = new_parent.left_child
        new_parent.left_child = elem
        new_parent.parent = grand_parent
        elem.right_child = new_child
        grand_parent = new_parent
        grand_parent.right_child = new_parent  # atualiza para o novo parent

        self.fix_height(elem, new_parent)
        return elem

    def right_rotate(self, elem):
        new_parent = elem.left_child
        new_child = new_parent.right_child
        new_parent.right_child = elem
        new_parent.parent = elem.parent
        elem.left_child = new_child
        elem.parent = new_parent

        self.fix_height(elem, new_parent)
        return new_parent

    def balancing(self, child, parent):
        if child.value > parent.value:
            parent.right_height += 1
        else:
            parent.left_height += 1
        if parent.right_height == parent.left_height:
            # se as alturas forem iguais, então não vão afetar
            # a altura dos nós anteriores
            return
        balancing_factor_p = parent.right_height - parent.left_height
        balancing_factor_c = child.right_height - child.left_height
        if balancing_factor_p <= -2:
            if balancing_factor_c >= 1:
                # se o desbalanceamento estiver para dentro, realiza duas
                # rotacoes (child e parent)
                self.left_rotate(child)
            parent = self.right_rotate(parent)
            print("rotacao p:", parent.value, " c:", parent.right_child.value)
            return parent
        elif balancing_factor_p >= 2:
            if balancing_factor_c <= -1:
                # se desbalanceamento estiver dentro, realiza duas
                # rotacoes (child e parent)
                self.right_rotate(child)
            parent = self.left_rotate(parent)
            # print("rotacao p:", parent.value, " c:", parent.left_child.value)
            return parent
        if parent.parent != None:
            return self.balancing(parent, parent.parent)
        else:
            # atualiza a raiz
            print("new = ", parent.value)
            self.root = parent
        return parent

    def insert_elem(self, elem):
        if isinstance(elem, BinaryElem):
            parent = self.search_pos_insert(elem)
            elem.parent = parent
            if parent != None:
                if elem.value > parent.value:
                    parent.right_child = elem
                    parent = self.balancing(elem, parent)
                else:
                    parent.left_child = elem
                    parent = self.balancing(elem, parent)
            else:
                # elem adicionado é a raiz se não houver outros elementos
                self.root = elem

    def in_order(self, elem=0):
        if elem == 0:
            elem = self.root
        if elem != None:
            self.in_order(elem.left_child)
            print(elem.value, " l:", elem.left_height,
                  " r:", elem.right_height, end=", ")
            self.in_order(elem.right_child)

    def pre_order(self, elem=0):
        if elem == 0:
            elem = self.root
        if elem != None:
            print(elem.value, end=" ")
            self.pre_order(elem.left_child)
            self.pre_order(elem.right_child)


def main():
    tree = BinaryTree()
    array = [20, 33, 5, 9, 3, 2, 56, 71]
    for elem in array:
        tree.insert_elem(BinaryElem(elem))
    print("In Order")
    tree.in_order()
    print()
    tree.pre_order()
    print()


main()
