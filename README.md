# Destroyer II

---

## 👤 Identitas Cakru

* Nama: Muhammad Rizqi Azhar
* NIM: 18225103

---

## Deskripsi Singkat

Program **Destroyer II** merupakan sistem berbasis ROS2 yang mengatur pergerakan robot menggunakan dua mode kendali:

* **Autonomous Mode** (`auto_mode_destroyer`)
* **Manual/Drive Mode** (`drive_mode_destroyer`)

Sistem ini menggunakan node **Priority Mux** untuk menentukan prioritas input yang akan digunakan. Output kemudian diproses oleh node **Move Destroyer** untuk menghasilkan arah gerakan roda.

---

## Diagram Sistem

```
[drive_mode_destroyer] ---> (/drive_destroyer) ----\
                                                    \
                                                     > [priority_mux_destroyer]
                                                    /
[auto_mode_destroyer] ---> (/autonomous_destroyer) /

[priority_mux_destroyer] ---> (/power_destroyer) ---> [move_destroyer]
                        ---> (/move_type) ---------> [move_destroyer]
```

**Penjelasan:**

* Node **priority_mux_destroyer** memilih antara input manual dan autonomous dengan prioritas di drive_mode_destroyer
* Jika tidak ada input dalam waktu tertentu, sistem akan idle
* Output dikirim ke **move_destroyer** untuk menentukan arah roda

---

## Dependencies

Program ini membutuhkan:

* ROS2 Humble
* `rclcpp`
* `geometry_msgs`
* `std_msgs`
* `colcon`

---

## Cara Menjalankan Program

### 1. Build Workspace

```bash
colcon build
source install/setup.bash
```

---

### 2. Jalankan dengan Launch File (Switch On)

```bash
ros2 launch destroyer switch_on.py
```

---

### 3. Manual

Jalankan masing-masing node:

```bash
ros2 run destroyer auto_mode_destroyer
ros2 run destroyer drivwe_mode_destroyer
ros2 run destroyer check_move_destroyer
ros2 run destroyer move_destroyer
```

---

