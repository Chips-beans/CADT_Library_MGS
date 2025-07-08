# generate_rooms_custom_order.py
with open('rooms.txt', 'w') as f:
    for floor in range(1, 10):       # Floors 1 to 9
        for room_num in range(0, 10): # Rooms 0 to 9 per floor
            room_id = floor * 100 + room_num
            
            # Determine room type
            if room_num <= 4:
                room_type = "Single"
            else:
                room_type = "Double"
            
            status = "Free"
            guest = "-"
            
            # Write in order: RoomNumber,RoomType,Status,GuestName
            f.write(f"{room_id},{room_type},{status},{guest}\n")

print("rooms.txt created with custom order fields.")
