Oto kompletna dokumentacja w jednym pliku Markdown, gotowa do skopiowania (np. do pliku `README.md` lub `API.md`). Wszystkie przykłady zawierają wypełnione dane (UUID, nazwy obrazków, wyniki) zamiast pustych pól.



````markdown
# Dobble - SK2 Documentation

## Development

### Prerequisites
> From inside project root folder
- `cmake -B build` - create build folder


### Building

> From inside project root folder
- `cmake --build build` - build full project
- `cmake --build build --target dobble_server` - build only server
- `cmake --build build --target dobble_client` - build only client

> From inside build folder
- `make` - build full project
- `make dobble_server` - build only server
- `make dobble_client` - build only client

---

## Communication Protocol

**Type:** TCP Socket
**Port:** 3333
**Format:** JSON

### General Message Structure

Every message follows this wrapper structure.

**Request (Client -> Server):**
```json
{
    "command": "command_name",
    "client_id": "client-uuid-1111-2222",
    "data": {
        "field1": "value"
    }
}
````

**Response (Server -\> Client):**

```json
{
    "command": "command_name",
    "server_id": "server-main-01",
    "data": {
        "result": "value"
    },
    "error": null
}
```

**Error Response (Server -\> Client):**

```json
{
    "command": "command_name",
    "server_id": "server-main-01",
    "data": null,
    "error": {
        "message": "Description of the error"
    }
}
```

-----

## API Endpoints

### 1\. Ping

Check connection.

**Request:**

```json
{
    "command": "ping",
    "client_id": "client-uuid-1111-2222",
    "data": {
        "message": "ping"
    }
}
```

**Response:**

```json
{
    "command": "ping",
    "server_id": "server-main-01",
    "data": {
        "message": "pong"
    },
    "error": null
}
```

### 2\. GetStatus

Get information about the current client state.

**Request:**

```json
{
    "command": "getstatus",
    "client_id": "client-uuid-1111-2222",
    "data": {
        "message": "getstatus"
    }
}
```

**Response:**

```json
{
    "command": "getstatus",
    "server_id": "server-main-01",
    "data": {
        "name": "PlayerOne",
        "status": "idle"
    },
    "error": null
}
```

### 3\. GetInfoLobby

Get list of available games.

**Request:**

```json
{
    "command": "getinfolobby",
    "client_id": "client-uuid-1111-2222",
    "data": {
        "page": "1"
    }
}
```

**Response:**

```json
{
    "command": "getinfolobby",
    "server_id": "server-main-01",
    "data": {
        "page": "1",
        "actual_games": [
            {
                "game_id": "game-uuid-8888",
                "game_name": "Room Alpha",
                "players": "2",
                "max_players": "4",
                "nicknames": ["PlayerOne", "PlayerTwo"],
                "status": "waiting"
            },
            {
                "game_id": "game-uuid-9999",
                "game_name": "Room Beta",
                "players": "4",
                "max_players": "4",
                "nicknames": ["Anna", "Bob", "Joe", "Mike"],
                "status": "ongoing"
            }
        ],
        "next_page": "2"
    },
    "error": null
}
```

### 4\. GetInfoGame

Get details about a specific game.

**Request:**

```json
{
    "command": "getinfogame",
    "client_id": "client-uuid-1111-2222",
    "data": {
        "game_id": "game-uuid-8888"
    }
}
```

**Response:**

```json
{
    "command": "getinfogame",
    "server_id": "server-main-01",
    "data": {
        "game_id": "game-uuid-8888",
        "game_name": "Room Alpha",
        "players": "2",
        "nicknames": ["PlayerOne", "PlayerTwo"],
        "status": "waiting"
    },
    "error": null
}
```

### 5\. JoinGame

Join a room as a player or observer.

**Request:**

```json
{
    "command": "join_game",
    "client_id": "client-uuid-1111-2222",
    "data": {
        "game_id": "game-uuid-8888",
        "role": "player"
    }
}
```

*(Role can be: "player" | "observator")*

**Response (Success):**

```json
{
    "command": "join_game",
    "server_id": "server-main-01",
    "data": {
        "status": "success",
        "role": "player",
        "game_info": {
            "game_id": "game-uuid-8888",
            "name": "Room Alpha"
        }
    },
    "error": null
}
```

**Response (Error):**

```json
{
    "command": "join_game",
    "server_id": "server-main-01",
    "data": null,
    "error": {
        "message": "You cannot join the game because it is full"
    }
}
```

-----

## Internal Data Structures

### Game Model (Server internal representation)

```json
{
    "game_id": "game-uuid-8888",
    "actual_turn": { "turn_id": "turn-101", "active": true },
    "past_turns": [
        { "turn_id": "turn-100", "winner": "client-uuid-3333","active":"false" }
    ],
    "clients_id_data": [
        { "client_id": "client-uuid-1111-2222", "points": 10, "mistakes": 1 },
        { "client_id": "client-uuid-3333-4444", "points": 15, "mistakes": 0 }
    ]
}
```

### Turn Structure

```json
{
    "actual_imgs": ["cactus.png", "sun.png", "igloo.png"],
    "players_imgs": [
        {
            "client_id": "client-uuid-1111-2222",
            "imgs": [
                { "img_name": "cactus.png", "rotate": "45","place":"1","size":"50%"},
                { "img_name": "moon.png", "rotate": "90","place":"2","size":"%"}
            ],
            
        }
    ]
}
```

-----

## Game Flow

**Sequence:** 1. `Client` Join Room
2\. `Server` Send Game Info (Start)
3\. `Client` Move
4\. `Server` Next Turn / Broadcast Result
5\. Repeat 3-4
6\. Game Over

### 1\. SendGameInfo (Server -\> Client)

Initial game state sent to client upon joining/starting.

```json
{
    "command": "sendgame_info",
    "server_id": "server-main-01",
    "data": {
        "game_id": "game-uuid-8888",
        "actual_turn": {
            "turn_id": "turn-101",
            "clients_data": [
                {
                    "client_id": "client-uuid-1111-2222",
                    "imgs": [
                        { "img_name": "cactus.png", "rotate": "45" },
                        { "img_name": "car.png", "rotate": "0" }
                    ],
                    "score": 10
                },
                {
                    "client_id": "client-uuid-3333-4444",
                    "imgs": [
                        { "img_name": "dog.png", "rotate": "180" },
                        { "img_name": "sun.png", "rotate": "90" }
                    ],
                    "score": 12
                }
            ],
            "images_on_table": [
                { "img_name": "cactus.png", "rotation": "12" },
                { "img_name": "apple.png", "rotation": "45" }
            ],
            "scoreboard": [
                { "client_id": "client-uuid-1111-2222", "score": "10" },
                { "client_id": "client-uuid-3333-4444", "score": "12" }
            ],
            "past_turns": [
                { "turn_id": "turn-100", "winner_id": "client-uuid-3333-4444" }
            ]
        }
    },
    "error": null
}
```

### 2\. Move (Client -\> Server)

Client attempts to match an image.

**Request:**

```json
{
    "command": "move",
    "client_id": "client-uuid-1111-2222",
    "data": {
        "turn_id": "turn-101",
        "game_id": "game-uuid-8888",
        "img_name": "cactus.png"
    }
}
```

**Response (Private confirmation to client):**

```json
{
    "command": "move",
    "server_id": "server-main-01",
    "client_id": "client-uuid-1111-2222",
    "data": {
        "game_id": "game-uuid-8888",
        "turn_id": "turn-101",
        "img_name": "cactus.png",
        "result": "success"
    },
    "error": null
}
```

*(Result: "success" | "miss")*

### 3\. Next Turn (Server -\> Broadcast)

Broadcast to all players when a turn ends (someone matched correctly).

```json
{
    "command": "next_turn",
    "server_id": "server-main-01",
    "game_id": "game-uuid-8888",
    "data": {
        "winner_id": "client-uuid-1111-2222",
        "actual_turn": {
            "turn_id": "turn-102",
            "clients_data": [
                 {
                    "client_id": "client-uuid-1111-2222",
                    "imgs": [{"img_name": "spider.png", "rotate": "10"}],
                    "score": 11
                },
                {
                    "client_id": "client-uuid-3333-4444",
                    "imgs": [{"img_name": "web.png", "rotate": "20"}],
                    "score": 12
                }
            ],
            "images_on_table": [
                { "img_name": "spider.png", "rotation": "300" },
                { "img_name": "bomb.png", "rotation": "15" }
            ],
            "scoreboard": [
                { "client_id": "client-uuid-1111-2222", "score": "11" },
                { "client_id": "client-uuid-3333-4444", "score": "12" }
            ]
        }
    },
    "error": null
}
```

### 4\. Leave Room

**Request:**

```json
{
    "command": "leaveroom",
    "game_id": "game-uuid-8888",
    "client_id": "client-uuid-1111-2222",
    "data": {
        "message": "leave"
    }
}
```

**Response (To the leaver):**

```json
{
    "command": "leaveroom",
    "server_id": "server-main-01",
    "game_id": "game-uuid-8888",
    "data": {
        "message": "ok"
    },
    "error": null
}
```

**Broadcast (To other players):**

```json
{
    "command": "clientleft",
    "server_id": "server-main-01",
    "game_id": "game-uuid-8888",
    "data": {
        "client_data": {
            "client_id": "client-uuid-1111-2222",
            "message": "leave"
        }
    },
    "error": null
}
```

### 5\. Game End

Server declares the game over.

```json
{
    "command": "gameend",
    "server_id": "server-main-01",
    "game_id": "game-uuid-8888",
    "data": {
        "client_data": {
            "client_id": "client-uuid-3333-4444",
            "score": 25,
            "rank": 1
        },
        "scoreboard": [
             { "client_id": "client-uuid-3333-4444", "score": "25" },
             { "client_id": "client-uuid-1111-2222", "score": "18" }
        ]
    },
    "error": null
}
```

```
```