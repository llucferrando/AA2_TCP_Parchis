# [TCP] Parchis / Ludo Game

![C++](https://img.shields.io/badge/C++-00599C?style=for-the-badge&logo=cplusplus&logoColor=white)
![TCP](https://img.shields.io/badge/TCP-5C2D91?style=for-the-badge&logo=network&logoColor=white)
![Visual Studio](https://img.shields.io/badge/Visual%20Studio-5C2D91.svg?style=for-the-badge&logo=visual-studio&logoColor=white)
![Windows](https://img.shields.io/badge/Windows-0078D6?style=for-the-badge&logo=windows&logoColor=white)

**GOAL** 

Develop an online multiplayer version of the classic board game Parchis using C++ and TCP sockets, with support for login/registration, lobby creation, and full gameplay rules.

**How we resolved it** 

We built the project in C++ with Visual Studio and SFML for rendering. The system was designed using an Entity Component System (ECS) architecture to keep the code modular and maintainable. The bootstrap server handled login, registration, and lobby management, while clients connected peer-to-peer to play the matches. We successfully implemented turn order, dice logic, piece movement, and win conditions. 

Some features, such as barrier mechanics and reconnection handling when a player leaves, were not fully implemented, but the core gameplay loop works correctly.

```markdown
 📞 Contact

Nahuel Aparicio Del Blanco

- GitHub: https://github.com/NahuelAparicio10
- Email: nahuel.ap.code@gmail.com
- LinkedIn: https://www.linkedin.com/in/nahuel-aparicio-del-blanco

Lluc Ferrando Jorda
- GitHub: 
- Email: 
- LinkedIn: 
