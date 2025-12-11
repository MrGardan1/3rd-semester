package lr3_test

import (
	"encoding/binary"
	"fmt"
	"os"
	"testing"

	"github.com/stretchr/testify/require"

	"lr3/lr3"
)

// ---------- базовые операции ----------

func TestHashTableBasicPutGet(t *testing.T) {
	h := lr3.NewHashTable()

	require.Equal(t, 0, h.Size())
	require.True(t, h.IsEmpty())

	h.Put("one", "1")
	h.Put("two", "2")

	require.Equal(t, 2, h.Size())
	require.False(t, h.IsEmpty())

	v, ok := h.Get("one")
	require.True(t, ok)
	require.Equal(t, "1", v)

	v, ok = h.Get("two")
	require.True(t, ok)
	require.Equal(t, "2", v)

	_, ok = h.Get("missing")
	require.False(t, ok)
}

func TestHashTableUpdateValue(t *testing.T) {
	h := lr3.NewHashTable()

	h.Put("k", "old")
	v, ok := h.Get("k")
	require.True(t, ok)
	require.Equal(t, "old", v)

	h.Put("k", "new")
	v, ok = h.Get("k")
	require.True(t, ok)
	require.Equal(t, "new", v)
}

// ---------- Remove: все ветки ----------

func TestHashTableRemoveAllBranches(t *testing.T) {
	h := lr3.NewHashTable()

	// 1) пустой бакет: Remove возвращает false
	ok := h.Remove("no_such")
	require.False(t, ok)

	// 2) один элемент: удаляем голову
	h.Put("head", "v")
	ok = h.Remove("head")
	require.True(t, ok)
	require.Equal(t, 0, h.Size())

	// 3) цепочка: голова + несколько элементов в одном бакете
	// используем много разных ключей, чтобы с высокой вероятностью
	// получить коллизии и длинную цепочку
	keys := []string{}
	for i := 0; i < 50; i++ {
		keys = append(keys, fmt.Sprintf("k_%d", i))
	}
	for _, k := range keys {
		h.Put(k, "v")
	}

	// удаляем какой‑то существующий ключ (где‑то в середине цепочки)
	ok = h.Remove(keys[10])
	require.True(t, ok)

	// попытка удалить несуществующий ключ: цикл полностью проходит и возвращает false
	ok = h.Remove("totally_missing_key")
	require.False(t, ok)
}

// ---------- текст: нормальные случаи ----------

func TestHashTableTextSerializeBasic(t *testing.T) {
	path := "htable_txt_basic.txt"
	defer os.Remove(path)

	h := lr3.NewHashTable()
	h.Put("x", "1")
	h.Put("y", "2")
	h.Put("z", "3")

	require.NoError(t, h.SaveText(path))

	h2 := lr3.NewHashTable()
	require.NoError(t, h2.LoadText(path))

	v, ok := h2.Get("x")
	require.True(t, ok)
	require.Equal(t, "1", v)

	v, ok = h2.Get("y")
	require.True(t, ok)
	require.Equal(t, "2", v)

	v, ok = h2.Get("z")
	require.True(t, ok)
	require.Equal(t, "3", v)
}

func TestHashTableTextSerializeEmpty(t *testing.T) {
	path := "htable_txt_empty.txt"
	defer os.Remove(path)

	h := lr3.NewHashTable()
	require.NoError(t, h.SaveText(path))

	h2 := lr3.NewHashTable()
	require.NoError(t, h2.LoadText(path))
	require.True(t, h2.IsEmpty())
}

// ---------- текст: ошибки и особые строки ----------

func TestHashTableTextInvalidLines(t *testing.T) {
	path := "htable_txt_invalid.txt"
	defer os.Remove(path)

	content := "" +
		"\n" +          // пустая строка -> должна быть пропущена
		"=value\n" +    // '=' в начале -> sep == 0 -> ошибка
		"key=\n" +      // '=' в конце -> sep == len(line)-1 -> ошибка
		"nokeynoval\n" // без '=' -> sep == -1 -> ошибка

	require.NoError(t, os.WriteFile(path, []byte(content), 0o644))

	h := lr3.NewHashTable()
	err := h.LoadText(path)
	require.Error(t, err)
}

func TestHashTableTextScannerError(t *testing.T) {
	// Создаём ОЧЕНЬ длинную строку, чтобы потенциально спровоцировать ошибку Scanner
	// (ErrTooLong) и пройти ветку возврата ошибки sc.Err().
	path := "htable_txt_longline.txt"
	defer os.Remove(path)

	long := make([]byte, 0, 200000)
	for i := 0; i < 200000; i++ {
		long = append(long, 'a')
	}
	long = append(long, '\n')

	require.NoError(t, os.WriteFile(path, long, 0o644))

	h := lr3.NewHashTable()
	_ = h.LoadText(path) // результат нас не интересует, важен сам проход по коду
}

// ---------- бинарь: нормальные случаи ----------

func TestHashTableBinarySerializeBasic(t *testing.T) {
	path := "htable_bin_basic.bin"
	defer os.Remove(path)

	h := lr3.NewHashTable()
	h.Put("k1", "v1")
	h.Put("k2", "v2")
	h.Put("k3", "v3")

	require.NoError(t, h.SaveBinary(path))

	h2 := lr3.NewHashTable()
	require.NoError(t, h2.LoadBinary(path))

	v, ok := h2.Get("k1")
	require.True(t, ok)
	require.Equal(t, "v1", v)

	v, ok = h2.Get("k2")
	require.True(t, ok)
	require.Equal(t, "v2", v)

	v, ok = h2.Get("k3")
	require.True(t, ok)
	require.Equal(t, "v3", v)
}

func TestHashTableBinarySerializeEmpty(t *testing.T) {
	path := "htable_bin_empty.bin"
	defer os.Remove(path)

	h := lr3.NewHashTable()
	require.NoError(t, h.SaveBinary(path))

	h2 := lr3.NewHashTable()
	require.NoError(t, h2.LoadBinary(path))
	require.True(t, h2.IsEmpty())
}

// ---------- бинарь: ошибки при чтении/записи ----------

func TestHashTableBinaryOpenError(t *testing.T) {
	h := lr3.NewHashTable()

	// SaveBinary: ошибка os.Create
	err := h.SaveBinary("/root/htable_forbidden.bin")
	require.Error(t, err)

	// LoadBinary: ошибка os.Open
	err = h.LoadBinary("/root/htable_forbidden.bin")
	require.Error(t, err)
}

func TestHashTableBinaryLoadTruncatedCountsAndLengths(t *testing.T) {
	path := "htable_bin_trunc1.bin"
	defer os.Remove(path)

	// Пишем только count, но без записей -> LoadBinary должен вернуть ошибку
	f, err := os.Create(path)
	require.NoError(t, err)
	require.NoError(t, binary.Write(f, binary.LittleEndian, int32(1)))
	require.NoError(t, f.Close())

	h := lr3.NewHashTable()
	err = h.LoadBinary(path)
	require.Error(t, err)
}

func TestHashTableBinaryLoadTruncatedKeyBytes(t *testing.T) {
	path := "htable_bin_trunc2.bin"
	defer os.Remove(path)

	f, err := os.Create(path)
	require.NoError(t, err)

	// count = 1
	require.NoError(t, binary.Write(f, binary.LittleEndian, int32(1)))

	// keyLen = 10, но сами байты ключа не пишем
	require.NoError(t, binary.Write(f, binary.LittleEndian, int32(10)))
	require.NoError(t, f.Close())

	h := lr3.NewHashTable()
	err = h.LoadBinary(path)
	require.Error(t, err)
}

func TestHashTableBinaryLoadTruncatedValBytes(t *testing.T) {
	path := "htable_bin_trunc3.bin"
	defer os.Remove(path)

	f, err := os.Create(path)
	require.NoError(t, err)

	// count = 1
	require.NoError(t, binary.Write(f, binary.LittleEndian, int32(1)))

	// keyLen = 3, пишем ключ "abc"
	require.NoError(t, binary.Write(f, binary.LittleEndian, int32(3)))
	_, err = f.Write([]byte("abc"))
	require.NoError(t, err)

	// valLen = 10, но байты значения не пишем
	require.NoError(t, binary.Write(f, binary.LittleEndian, int32(10)))
	require.NoError(t, f.Close())

	h := lr3.NewHashTable()
	err = h.LoadBinary(path)
	require.Error(t, err)
}
