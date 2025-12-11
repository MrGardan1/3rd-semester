package lr3

import (
	"bufio"
	"encoding/binary"
	"errors"
	"os"
)

type Array struct {
	data []string
}

func NewArray() *Array {
	return &Array{data: make([]string, 0)}
}

func (a *Array) Size() int {
	return len(a.data)
}

func (a *Array) Get(i int) (string, error) {
	if i < 0 || i >= len(a.data) {
		return "", errors.New("index out of range")
	}
	return a.data[i], nil
}

func (a *Array) Set(i int, v string) error {
	if i < 0 || i >= len(a.data) {
		return errors.New("index out of range")
	}
	a.data[i] = v
	return nil
}

func (a *Array) PushBack(v string) {
	a.data = append(a.data, v)
}

func (a *Array) PushFront(v string) {
	a.data = append([]string{v}, a.data...)
}

func (a *Array) InsertAt(i int, v string) error {
	if i < 0 || i > len(a.data) {
		return errors.New("index out of range")
	}
	a.data = append(a.data[:i], append([]string{v}, a.data[i:]...)...)
	return nil
}

func (a *Array) PopBack() (string, error) {
	n := len(a.data)
	if n == 0 {
		return "", errors.New("empty array")
	}
	v := a.data[n-1]
	a.data = a.data[:n-1]
	return v, nil
}

func (a *Array) PopFront() (string, error) {
	if len(a.data) == 0 {
		return "", errors.New("empty array")
	}
	v := a.data[0]
	a.data = a.data[1:]
	return v, nil
}

// ---------- текстовая сериализация ----------

func (a *Array) SaveText(path string) error {
	f, err := os.Create(path)
	if err != nil {
		return err
	}
	defer f.Close()

	w := bufio.NewWriter(f)
	for _, s := range a.data {
		if _, err := w.WriteString(s + "\n"); err != nil {
			return err
		}
	}
	return w.Flush()
}

func (a *Array) LoadText(path string) error {
	f, err := os.Open(path)
	if err != nil {
		return err
	}
	defer f.Close()

	a.data = a.data[:0]
	sc := bufio.NewScanner(f)
	for sc.Scan() {
		a.data = append(a.data, sc.Text())
	}
	return sc.Err()
}

// ---------- бинарная сериализация ----------

func (a *Array) SaveBinary(path string) error {
	f, err := os.Create(path)
	if err != nil {
		return err
	}
	defer f.Close()

	if err := binary.Write(f, binary.LittleEndian, int32(len(a.data))); err != nil {
		return err
	}
	for _, s := range a.data {
		b := []byte(s)
		if err := binary.Write(f, binary.LittleEndian, int32(len(b))); err != nil {
			return err
		}
		if _, err := f.Write(b); err != nil {
			return err
		}
	}
	return nil
}

func (a *Array) LoadBinary(path string) error {
	f, err := os.Open(path)
	if err != nil {
		return err
	}
	defer f.Close()

	a.data = a.data[:0]

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
		a.data = append(a.data, string(buf))
	}
	return nil
}