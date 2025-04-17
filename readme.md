# 🧾 C++ HTML & CSS Parser & Renderer

This project is a **C++-based HTML and CSS parser and renderer**, developed as a course project. It parses a static HTML file and applies styles defined in a CSS file. The output is rendered in a graphical window using **SDL2** and **SDL_ttf** libraries, simulating a basic browser rendering engine.

---

## 📌 Features Implemented

This project supports parsing and rendering of the following:

### ✅ HTML Tags
- `<html>`, `<head>`, `<body>`
- `<h1>`, `<h2>` (Headings)
- `<div>` (Blocks/Sections)
- `<p>` (Paragraphs)

### ✅ CSS Features
- `color`, `background-color`
- `margin`, `padding`
- `font-size`
- `hover` effects:
  - Text color change
  - Background color change

---

## 🎯 Project Capabilities

This parser and renderer can handle:

- Nested HTML structure
- Class-based CSS styling (e.g., `.child1`, `.parent1`, etc.)
- Inline layout simulation
- Hover effect simulation
- Simplified box model (margin + padding)
- Consistent color rendering using SDL

---

## 🛠 How to Run

### 🔁 Clone the repository

```bash
git clone https://github.com/your-username/cpp-html-css-renderer.git
cd cpp-html-css-renderer
```

### 💻 Open in Visual Studio

1. Open the `.sln` file in Visual Studio.
2. Set the project as startup.
3. **Configure Environment Variables** for SDL:
   - Add paths to `SDL2` and `SDL_ttf` headers and libraries in:
     - Project Properties > VC++ Directories
     - Project Properties > Linker > Additional Dependencies
4. Build and Run.

> Make sure the sample HTML (`sample.html`) and CSS (`style.css`) files are in the correct directory.

---

## 🖼 Rendering Previews

### ✔ Browser View



### ✔ HTMLer Output
![HTMlLer](https://github.com/user-attachments/assets/cd5f42ce-998d-4ea9-bbd4-dbc3245714ee)


---

## 📚 Supported CSS Properties

| Property           | Supported |
|--------------------|-----------|
| `margin`           | ✅        |
| `padding`          | ✅        |
| `color`            | ✅        |
| `background-color` | ✅        |
| `font-size`        | ✅        |
| `:hover` effects   | ✅        |

---

## 🔧 Future Improvements

- Add support for more HTML tags like `<ul>`, `<li>`, `<a>`, `<img>`
- Advanced CSS features like `border`, `display`, `flex`, etc.
- JavaScript support for basic interactivity
- Improved rendering engine and layout management

---

## 📦 Dependencies

- [SDL2](https://www.libsdl.org/)
- [SDL_ttf](https://www.libsdl.org/projects/SDL_ttf/)

---

## 🧠 Learning Objectives

- Understand parsing of structured text (HTML & CSS)
- Explore how rendering engines interpret code
- Develop low-level graphical rendering using C++
- Improve code organization and architecture for interpreters

---
## 📃 Created By
- [Madhav Parajuli](http://mpparajuli.com.np)
- [Madhu Sharma](http://madhusharma.com.np)


## 📃 License

This project is intended for educational purposes only. All content is © 2025.

---

Feel free to fork and expand the renderer to support more tags, styling rules, and interactions!

