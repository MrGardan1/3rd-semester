package lr3

import (
	"bufio"
	"encoding/binary"
	"errors"
	"hash/fnv"
	"os"
)

type htEntry struct {
	key   string
	value string
	next  *htEntry
}

type HashTable struct {
	buckets []*htEntry
	size    int
}

func NewHashTable() *HashTable {
	// фиксированное число бакетов для простоты
	return &HashTable{
		buckets: make([]*htEntry, 16),
	}
}

func (h *HashTable) Size() int {
	return h.size
}

func (h *HashTable) IsEmpty() bool {
	return h.size == 0
}

func (h *HashTable) hash(key string) int {
	hasher := fnv.New32a()
	_, _ = hasher.Write([]byte(key))
	return int(hasher.Sum32() % uint32(len(h.buckets)))
}

// Put добавляет или обновляет значение по ключу.
func (h *HashTable) Put(key, value string) {
	idx := h.hash(key)
	head := h.buckets[idx]

	// обновление существующего
	for e := head; e != nil; e = e.next {
		if e.key == key {
			e.value = value
			return
		}
	}

	// вставка в голову цепочки
	newEntry := &htEntry{key: key, value: value, next: head}
	h.buckets[idx] = newEntry
	h.size++
}

// Get возвращает значение и флаг, найден ключ или нет.
func (h *HashTable) Get(key string) (string, bool) {
	idx := h.hash(key)
	for e := h.buckets[idx]; e != nil; e = e.next {
		if e.key == key {
			return e.value, true
		}
	}
	return "", false
}

// Remove удаляет элемент по ключу, возвращает true, если что‑то удалили.
func (h *HashTable) Remove(key string) bool {
	idx := h.hash(key)
	head := h.buckets[idx]
	if head == nil {
		return false
	}

	if head.key == key {
		h.buckets[idx] = head.next
		h.size--
		return true
	}

	prev := head
	for e := head.next; e != nil; e = e.next {
		if e.key == key {
			prev.next = e.next
			h.size--
			return true
		}
		prev = e
	}
	return false
}

// ---------- текстовая сериализация ----------

// Формат: по строке "key=value" на элемент.
func (h *HashTable) SaveText(path string) error {
	f, err := os.Create(path)
	if err != nil {
		return err
	}
	defer f.Close()

	w := bufio.NewWriter(f)
	for _, head := range h.buckets {
		for e := head; e != nil; e = e.next {
			if _, err := w.WriteString(e.key + "=" + e.value + "\n"); err != nil {
				return err
			}
		}
	}
	return w.Flush()
}

func (h *HashTable) LoadText(path string) error {
	f, err := os.Open(path)
	if err != nil {
		return err
	}
	defer f.Close()

	// очищаем таблицу
	h.buckets = make([]*htEntry, len(h.buckets))
	h.size = 0

	sc := bufio.NewScanner(f)
	for sc.Scan() {
		line := sc.Text()
		if line == "" {
			continue
		}
		sep := -1
		for i := 0; i < len(line); i++ {
			if line[i] == '=' {
				sep = i
				break
			}
		}
		if sep <= 0 || sep == len(line)-1 {
			return errors.New("invalid line in text file")
		}
		key := line[:sep]
		val := line[sep+1:]
		h.Put(key, val)
	}
	return sc.Err()
}

// ---------- бинарная сериализация ----------
//
// Формат:
// int32 count
//   для каждого:
//     int32 keyLen, []byte(key)
//     int32 valLen, []byte(val)

func (h *HashTable) SaveBinary(path string) error {
	f, err := os.Create(path)
	if err != nil {
		return err
	}
	defer f.Close()

	if err := binary.Write(f, binary.LittleEndian, int32(h.size)); err != nil {
		return err
	}
	for _, head := range h.buckets {
		for e := head; e != nil; e = e.next {
			kb := []byte(e.key)
			vb := []byte(e.value)
			if err := binary.Write(f, binary.LittleEndian, int32(len(kb))); err != nil {
				return err
			}
			if _, err := f.Write(kb); err != nil {
				return err
			}
			if err := binary.Write(f, binary.LittleEndian, int32(len(vb))); err != nil {
				return err
			}
			if _, err := f.Write(vb); err != nil {
				return err
			}
		}
	}
	return nil
}

func (h *HashTable) LoadBinary(path string) error {
	f, err := os.Open(path)
	if err != nil {
		return err
	}
	defer f.Close()

	h.buckets = make([]*htEntry, len(h.buckets))
	h.size = 0

	var count int32
	if err := binary.Read(f, binary.LittleEndian, &count); err != nil {
		return err
	}
	for i := int32(0); i < count; i++ {
		var klen int32
		if err := binary.Read(f, binary.LittleEndian, &klen); err != nil {
			return err
		}
		kb := make([]byte, klen)
		if _, err := f.Read(kb); err != nil {
			return err
		}

		var vlen int32
		if err := binary.Read(f, binary.LittleEndian, &vlen); err != nil {
			return err
		}
		vb := make([]byte, vlen)
		if _, err := f.Read(vb); err != nil {
			return err
		}
		h.Put(string(kb), string(vb))
	}
	return nil
}
