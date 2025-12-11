package lr3

import (
	"bufio"
	"encoding/binary"
	"errors"
	"os"
)

type Stack struct {
	data []string
}

func NewStack() *Stack {
	return &Stack{data: make([]string, 0)}
}

func (s *Stack) Size() int {
	return len(s.data)
}

func (s *Stack) Push(v string) {
	s.data = append(s.data, v)
}

func (s *Stack) Pop() (string, error) {
	n := len(s.data)
	if n == 0 {
		return "", errors.New("empty stack")
	}
	v := s.data[n-1]
	s.data = s.data[:n-1]
	return v, nil
}

func (s *Stack) Peek() (string, error) {
	n := len(s.data)
	if n == 0 {
		return "", errors.New("empty stack")
	}
	return s.data[n-1], nil
}

// ---------- текстовая сериализация ----------

func (s *Stack) SaveText(path string) error {
	f, err := os.Create(path)
	if err != nil {
		return err
	}
	defer f.Close()

	w := bufio.NewWriter(f)
	for _, v := range s.data {
		if _, err := w.WriteString(v + "\n"); err != nil {
			return err
		}
	}
	return w.Flush()
}

func (s *Stack) LoadText(path string) error {
	f, err := os.Open(path)
	if err != nil {
		return err
	}
	defer f.Close()

	s.data = s.data[:0]
	sc := bufio.NewScanner(f)
	for sc.Scan() {
		s.data = append(s.data, sc.Text())
	}
	return sc.Err()
}

// ---------- бинарная сериализация ----------

func (s *Stack) SaveBinary(path string) error {
	f, err := os.Create(path)
	if err != nil {
		return err
	}
	defer f.Close()

	if err := binary.Write(f, binary.LittleEndian, int32(len(s.data))); err != nil {
		return err
	}
	for _, v := range s.data {
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

func (s *Stack) LoadBinary(path string) error {
	f, err := os.Open(path)
	if err != nil {
		return err
	}
	defer f.Close()

	s.data = s.data[:0]

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
		s.data = append(s.data, string(buf))
	}
	return nil
}
