### https://www.tomarkdown.org/guides/markdown-complete-guide
# Level 1 Header (H1)

## Level 2 Header (H2)

### Level 3 Header (H3)

#### Level 4 Header (H4)

##### Level 5 Header (H5)

###### Level 6 Header (H6)


---
### separate with 1 blank line
This is the first paragraph. It can contain multiple lines,
and as long as no blank lines separate them, they form one paragraph.

This is the second paragraph. Note the blank line above,
which creates a new paragraph.

---
### add two spaces at line end
This is line one.  
This is line two, within the same paragraph.

---
### Text Emphasis
_This is italic text_  
_This is also italic text_

---
### Bold Text
**This is bold text**
**This is also bold text**


---
### Strike Through
~~This is strikethrough text~~

---
### Lists: Unordered lists use -, +, or * as markers:
- First item
- Second item
- Third item
    - Nested item
        - more
    - Another nested item
- Fourth item

1. First item
2. Second item
3. Third item
    1. Nested ordered item
        1. more
    2. Another nested ordered item
4. Fourth item

#### numbers don't matter in ordered lists
1. First item
1. Second item
1. Third item

---
### exp
```js
function sayHello() {
    console.log("Hello, world!");
}
sayHello();
```

---
### Blockquotes
> This is a blockquote.
> It can span multiple lines.
>
> Even multiple paragraphs.

> Nested quotes
>
> > This is a nested quote
> >
> > > Deeper nesting


---
### Escape Characters
\*Not italic text\*
\# Not a heading
\[Not a link\]
#### Common characters needing escaping: *, _, #, [, ], (, ), !, etc.

---
# Advanced Markdown Techniques
### Tables
| Feature         | Markdown   | HTML       | Word     |
| --------------- | ---------- | ---------- | -------- |
| Ease of Use     | ⭐⭐⭐⭐⭐ | ⭐⭐       | ⭐⭐⭐⭐ |
| Compatibility   | ⭐⭐⭐⭐⭐ | ⭐⭐⭐⭐⭐ | ⭐⭐     |
| Version Control | ⭐⭐⭐⭐⭐ | ⭐⭐⭐⭐   | ⭐       |

| Left-aligned | Centered | Right-aligned |
| :----------- | :------: | ------------: |
| Content      | Content  |       Content |

---
### Task Lists
- [x] Completed task
- [ ] Incomplete task
- [x] Another completed task
- [ ] Pending task

---
### Footnotes
This sentence contains a footnote[^1].

[^1]: Footnote content here.

---
### Definition Lists
Term 1
: Definition 1

Term 2
: Definition 2a
: Definition 2b

---
### Mathematical Formulas
Inline formula: $E = mc^2$

Block formula:

$$
\sum_{i=1}^{n} x_i = x_1 + x_2 + \cdots + x_n
$$

---
### exp

---
### exp
