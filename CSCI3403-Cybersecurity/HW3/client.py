"""
    client.py - Connect to an SSL server

    CSCI 3403
    Authors: Matt Niemiec and Abigail Fernandes
    Number of lines of code in solution: 117
        (Feel free to use more or less, this
        is provided as a sanity check)

    Put your team members' names:
    Elsa Velazquez, Erika Bailon, Adam Ten Hoeve, Katrina Schwarzenberger, Gouri Yerra, Dhwani Khatter


"""

import socket
import random
import Crypto
import hashlib
import uuid
from Crypto.Cipher import AES
from Crypto.Hash import HMAC
from Crypto.Protocol.KDF import PBKDF2
from Crypto.PublicKey import RSA
#import PyCrypto

iv = "G4XO4L\X<J;MPPLD"

host = "localhost"
port = 10001


# A helper function that you may find useful for AES encryption
def pad_message(message):
    return message + " "*((16-len(message))%16)


# TODO: Generate a random AES key
def generate_key():
    # random.seed()
    # generated_key = [ random.randint(0, 100)  for k in range(10) ]
    random_key = os.urandom(16)
    return random_key
    #pass


# TODO: Takes an AES session key and encrypts it using the server's
# TODO: public key and returns the value
def encrypt_handshake(session_key):
    f = open('ssh-key')
    public_key = f.read()
    f.close()
    public_key = RSA.importKey(public_key)
    encrypted_session_key = public_key.encrypt(session_key, 0)
    return encrypted_session_key[0]
    #pass


# TODO: Encrypts the message using AES. Same as server function
def encrypt_message(message, session_key):
    pass



# TODO: Decrypts the message using AES. Same as server function
def decrypt_message(message, session_key):
    pass


# Sends a message over TCP
def send_message(sock, message):
    sock.sendall(message)


# Receive a message from TCP
def receive_message(sock):
    data = sock.recv(1024)
    return data


def main():
    user = input("What's your username? ")
    password = input("What's your password? ")

    # Create a TCP/IP socket
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    # Connect the socket to the port where the server is listening
    server_address = (host, port)
    print('connecting to {} port {}'.format(*server_address))
    sock.connect(server_address)

    try:
        # Message that we need to send
        message = user + ' ' + password

        # TODO: Generate random AES key
        session_key = generate_key()


        # TODO: Encrypt the session key using server's public key
        encrypt_handshake(session_key)


        # TODO: Initiate handshake
        send_message(sock, message)


        # Listen for okay from server (why is this necessary?)
        if receive_message(sock).decode() != "okay":
            print("Couldn't connect to server")
            exit(0)

        # TODO: Encrypt message and send to server

        # TODO: Receive and decrypt response from server and print

    finally:
        print('closing socket')
        sock.close()


if __name__ in "__main__":
    main()
