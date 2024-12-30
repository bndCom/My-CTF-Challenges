import pickle
from instagrapi import Client

with open(".client", 'rb') as f:
    client = pickle.load(f)

dst_id = client.user_id_from_username("0x4n3s")
print(dst_id)

threads = client.direct_threads()

for thread in threads:
    for user in thread.users:
        if dst_id == user.pk:
            print(f"Messages with {user.username}:")
            
            # getting messages
            for message in client.direct_messages(thread.id):
                print(f"{message.user_id}: {message.text}")
            break
