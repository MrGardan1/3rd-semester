package lr3

import (
	"bufio"
	"encoding/binary"
	"errors"
	"os"
)

type singlyNode struct {
	data string
	next *singlyNode
}

type SinglyList struct {
	head *singlyNode
	tail *singlyNode
	size int
}

func NewSinglyList() *SinglyList {
	return &SinglyList{}
}

func (l *SinglyList) Size() int {
	return l.size
}

func (l *SinglyList) IsEmpty() bool {
	return l.size == 0
}

func (l *SinglyList) Clear() {
	for l.head != nil {
		tmp := l.head
		l.head = l.head.next
		_ = tmp
	}
	l.tail = nil
	l.size = 0
}

func (l *SinglyList) PushFront(v string) {
	n := &singlyNode{data: v, next: l.head}
	l.head = n
	if l.tail == nil {
		l.tail = n
	}
	l.size++
}

func (l *SinglyList) PushBack(v string) {
	n := &singlyNode{data: v}
	if l.tail == nil {
		l.head = n
		l.tail = n
	} else {
		l.tail.next = n
		l.tail = n
	}
	l.size++
}

func (l *SinglyList) PopFront() (string, error) {
	if l.head == nil {
		return "", errors.New("empty list")
	}
	v := l.head.data
	l.head = l.head.next
	if l.head == nil {
		l.tail = nil
	}
	l.size--
	return v, nil
}

func (l *SinglyList) PopBack() (string, error) {
	if l.head == nil {
		return "", errors.New("empty list")
	}
	if l.head == l.tail {
		v := l.head.data
		l.head = nil
		l.tail = nil
		l.size--
		return v, nil
	}
	cur := l.head
	for cur.next != nil && cur.next != l.tail {
		cur = cur.next
	}
	v := l.tail.data
	cur.next = nil
	l.tail = cur
	l.size--
	return v, nil
}

func (l *SinglyList) InsertAfter(target, v string) bool {
	cur := l.head
	for cur != nil {
		if cur.data == target {
			n := &singlyNode{data: v, next: cur.next}
			cur.next = n
			if l.tail == cur {
				l.tail = n
			}
			l.size++
			return true
		}
		cur = cur.next
	}
	return false
}

func (l *SinglyList) InsertBefore(target, v string) bool {
	if l.head == nil {
		return false
	}
	if l.head.data == target {
		l.PushFront(v)
		return true
	}
	cur := l.head
	for cur.next != nil {
		if cur.next.data == target {
			n := &singlyNode{data: v, next: cur.next}
			cur.next = n
			l.size++
			return true
		}
		cur = cur.next
	}
	return false
}

func (l *SinglyList) RemoveByValue(v string) bool {
	if l.head == nil {
		return false
	}
	if l.head.data == v {
		_, _ = l.PopFront()
		return true
	}
	cur := l.head
	for cur.next != nil {
		if cur.next.data == v {
			tmp := cur.next
			cur.next = tmp.next
			if tmp == l.tail {
				l.tail = cur
			}
			l.size--
			return true
		}
		cur = cur.next
	}
	return false
}

func (l *SinglyList) Search(v string) bool {
	cur := l.head
	for cur != nil {
		if cur.data == v {
			return true
		}
		cur = cur.next
	}
	return false
}

// ---------- текстовая сериализация ----------

func (l *SinglyList) SaveText(path string) error {
	f, err := os.Create(path)
	if err != nil {
		return err
	}
	defer f.Close()

	w := bufio.NewWriter(f)
	cur := l.head
	for cur != nil {
		if _, err := w.WriteString(cur.data + "\n"); err != nil {
			return err
		}
		cur = cur.next
	}
	return w.Flush()
}

func (l *SinglyList) LoadText(path string) error {
	f, err := os.Open(path)
	if err != nil {
		return err
	}
	defer f.Close()

	l.Clear()
	sc := bufio.NewScanner(f)
	for sc.Scan() {
		l.PushBack(sc.Text())
	}
	return sc.Err()
}

// ---------- бинарная сериализация ----------

func (l *SinglyList) SaveBinary(path string) error {
	f, err := os.Create(path)
	if err != nil {
		return err
	}
	defer f.Close()

	if err := binary.Write(f, binary.LittleEndian, int32(l.size)); err != nil {
		return err
	}
	cur := l.head
	for cur != nil {
		b := []byte(cur.data)
		if err := binary.Write(f, binary.LittleEndian, int32(len(b))); err != nil {
			return err
		}
		if _, err := f.Write(b); err != nil {
			return err
		}
		cur = cur.next
	}
	return nil
}

func (l *SinglyList) LoadBinary(path string) error {
	f, err := os.Open(path)
	if err != nil {
		return err
	}
	defer f.Close()

	l.Clear()

	var n int32
	if err := binary.Read(f, binary.LittleEndian, &n); err != nil {
		return err
	}
	for i := int32(0); i < n; i++ {
		var lgt int32
		if err := binary.Read(f, binary.LittleEndian, &lgt); err != nil {
			return err
		}
		buf := make([]byte, lgt)
		if _, err := f.Read(buf); err != nil {
			return err
		}
		l.PushBack(string(buf))
	}
	return nil
}
