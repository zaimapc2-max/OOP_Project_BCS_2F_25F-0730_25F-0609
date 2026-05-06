<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>OOP Chess Game</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            margin: 40px;
            background-color: #0f172a;
            color: #e5e7eb;
            line-height: 1.6;
        }
        h1, h2 {
            color: #38bdf8;
        }
        code {
            background-color: #1e293b;
            padding: 4px 6px;
            border-radius: 5px;
        }
        pre {
            background-color: #1e293b;
            padding: 10px;
            border-radius: 8px;
            overflow-x: auto;
        }
        ul {
            margin-left: 20px;
        }
        .section {
            margin-bottom: 30px;
        }
    </style>
</head>
<body>

<h1>♟️ OOP Chess Game (C++)</h1>

<p>
A console-based Chess game developed using <strong>Object-Oriented Programming (OOP)</strong> principles in C++.
This project was built as a final project for our 2nd semester to demonstrate core OOP concepts like
<strong>encapsulation, inheritance, polymorphism, and abstraction</strong>.
</p>

<div class="section">
    <h2>📌 Features</h2>
    <ul>
        <li>Complete 8x8 Chess board</li>
        <li>All standard Chess pieces:
            <ul>
                <li>Pawn</li>
                <li>Rook</li>
                <li>Knight</li>
                <li>Bishop</li>
                <li>Queen</li>
                <li>King</li>
            </ul>
        </li>
        <li>Two-player turn-based gameplay</li>
        <li>Move validation based on Chess rules</li>
        <li>Piece capturing system</li>
        <li>Illegal move prevention</li>
        <li>Colored console output using ANSI escape codes</li>
        <li>Continuous gameplay loop</li>
    </ul>
</div>

<div class="section">
    <h2>🧠 OOP Concepts Used</h2>
    <ul>
        <li><strong>Encapsulation</strong> → Each class manages its own data and methods</li>
        <li><strong>Inheritance</strong> → Base class <code>Piece</code> with derived classes</li>
        <li><strong>Polymorphism</strong> → Virtual functions for movement logic</li>
        <li><strong>Abstraction</strong> → Separation of game logic</li>
    </ul>
</div>

<div class="section">
    <h2>🏗️ Project Structure</h2>
<pre>
Chess-OOP/
│
├── main.cpp
├── Board.h / Board.cpp
├── Piece.h / Piece.cpp
├── Pawn.h / Pawn.cpp
├── Rook.h / Rook.cpp
├── Knight.h / Knight.cpp
├── Bishop.h / Bishop.cpp
├── Queen.h / Queen.cpp
├── King.h / King.cpp
│
└── README.md
</pre>
</div>

<div class="section">
    <h2>▶️ How to Run</h2>

    <h3>1. Clone the repository</h3>
<pre>
git clone https://github.com/your-username/chess-oop.git
cd chess-oop
</pre>

    <h3>2. Compile</h3>
<pre>
g++ *.cpp -o chess
</pre>

    <h3>3. Run</h3>
<pre>
./chess
</pre>
</div>

<div class="section">
    <h2>🎮 How to Play</h2>
    <ul>
        <li>This is a 2-player game</li>
        <li>Players take turns entering moves</li>
        <li>Example input:
            <pre>e2 e4</pre>
        </li>
        <li>The game validates moves according to Chess rules</li>
    </ul>
</div>

<div class="section">
    <h2>⚠️ Limitations</h2>
    <ul>
        <li>No AI opponent</li>
        <li>Console-based only (no GUI)</li>
        <li>Some advanced rules may not be implemented:
            <ul>
                <li>Castling</li>
                <li>En passant</li>
                <li>Pawn promotion</li>
            </ul>
        </li>
    </ul>
</div>

<div class="section">
    <h2>🚀 Future Improvements</h2>
    <ul>
        <li>Add AI opponent (Minimax algorithm)</li>
        <li>Build GUI using SFML or Qt</li>
        <li>Save & load game feature</li>
        <li>Online multiplayer</li>
        <li>Complete all Chess rules</li>
    </ul>
</div>

<div class="section">
    <h2>👩‍💻 Authors</h2>
    <ul>
        <li><strong>Zaima Amjad</strong></li>
        <li><strong>[Teammate Name]</strong></li>
    </ul>
</div>

<div class="section">
    <h2>📚 Learning Outcomes</h2>
    <ul>
        <li>Applied OOP concepts in a real project</li>
        <li>Managed multiple interacting classes</li>
        <li>Implemented game logic</li>
        <li>Improved debugging skills</li>
    </ul>
</div>

<div class="section">
    <h2>⭐ Acknowledgment</h2>
    <p>
        This project was developed as part of our university coursework for learning
        Object-Oriented Programming in C++.
    </p>
</div>

</body>
</html>
