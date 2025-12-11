package lr3

import (
	"bufio"
	"encoding/binary"
	"errors"
	"os"
)

type Queue struct {
	data []string
}

func NewQueue() *Queue {
	return &Queue{data: make([]string, 0)}
}

func (q *Queue) Size() int {
	return len(q.data)
}

func (q *Queue) IsEmpty() bool {
	return len(q.data) == 0
}

func (q *Queue) Enqueue(v string) {
	q.data = append(q.data, v)
}

func (q *Queue) Dequeue() (string, error) {
	if len(q.data) == 0 {
		return "", errors.New("empty queue")
	}
	v := q.data[0]
	q.data = q.data[1:]
	return v, nil
}

func (q *Queue) Peek() (string, error) {
	if len(q.data) == 0 {
		return "", errors.New("empty queue")
	}
	return q.data[0], nil
}

// ---------- текстовая сериализация ----------

func (q *Queue) SaveText(path string) error {
	f, err := os.Create(path)
	if err != nil {
		return err
	}
	defer f.Close()

	w := bufio.NewWriter(f)
	for _, v := range q.data {
		if _, err := w.WriteString(v + "\n"); err != nil {
			return err
		}
	}
	return w.Flush()
}

func (q *Queue) LoadText(path string) error {
	f, err := os.Open(path)
	if err != nil {
		return err
	}
	defer f.Close()

	q.data = q.data[:0]
	sc := bufio.NewScanner(f)
	for sc.Scan() {
		q.data = append(q.data, sc.Text())
	}
	return sc.Err()
}

// ---------- бинарная сериализация ----------

func (q *Queue) SaveBinary(path string) error {
	f, err := os.Create(path)
	if err != nil {
		return err
	}
	defer f.Close()

	if err := binary.Write(f, binary.LittleEndian, int32(len(q.data))); err != nil {
		return err
	}
	for _, v := range q.data {
		b := []byte(v)
		if err := binary.Write(f, binary.LittleEndian, int32(len(b))); err != nil {
			return err
		}
		if _, err := f.Write(b); err != nil {
			return err
		}
	}
	return nil
}

func (q *Queue) LoadBinary(path string) error {
	f, err := os.Open(path)
	if err != nil {
		return err
	}
	defer f.Close()

	q.data = q.data[:0]

	var n int32
	if err := binary.Read(f, binary.LittleEndian, &n); err != nil {
		return err
	}
	for i := int32(0); i < n; i++ {
		var l int32
		if err := binary.Read(f, binary.LittleEndian, &l); err != nil {
			return err
		}
		buf := make([]byte, l)
		if _, err := f.Read(buf); err != nil {
			return err
		}
		q.data = append(q.data, string(buf))
	}
	return nil
}
