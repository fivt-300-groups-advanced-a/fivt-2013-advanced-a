#ifndef UPD_INFO_H

#define UPD_INFO_H

template <class T, class Summator, T neutral_sum>
class UpdInfoAssignmentAdd {
    public:

        UpdInfoAssignmentAdd() {
            is_assigned = false;
            added = neutral_sum;
        }

        UpdInfoAssignmentAdd
            (const T &new_assigned, const T &new_added, bool new_is_assigned) {
            is_assigned = new_is_assigned;
            assigned = new_assigned;
            added = new_added;
        }

        void push(const UpdInfoAssignmentAdd &from, int l, int r) {
            if (from.is_assigned) {
                assigned = from.assigned;
                added = from.added;
                is_assigned = from.is_assigned;
            } else {
                added = sum(added, from.added);
            }
        }

        T assigned, added;
        bool is_assigned;
        Summator sum;
};

template <class T> 
class UpdInfoAssignment {
    public:

        UpdInfoAssignment() {
            is_assigned = false;
        }

        explicit UpdInfoAssignment(const T &new_assigned) {
            assigned = new_assigned;
            is_assigned = true;
        }

        void push(const UpdInfoAssignment &from, int l, int r) {
            assigned = from.assigned;
            is_assigned = true;
        }

        T assigned;
        bool is_assigned;
};

template <class T, class Summator, T neutral_sum>
class UpdInfoAdd {
    public:

        UpdInfoAdd() {
            added = neutral_sum;
        }

        explicit UpdInfoAdd(const T &new_added) {
            added = new_added;
        }

        void push(const UpdInfoAdd &from, int l, int r) {
            added = sum(added, from.added);
        }

        T added;
        Summator sum;
};

#endif
