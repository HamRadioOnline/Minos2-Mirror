# Stable radio ports (udev)

`ttyUSB<n>` numbering is assigned in connection order, not per device, so
with more than one USB-to-serial radio the port Minos was configured with
can point at the wrong radio after a reboot or a reconnect. `MinosSetRadioPorts`
and `99-hamlib.rules.example` fix that: a udev rule creates a fixed
`/dev/<name>` symlink for each radio's USB serial, and the script re-points
`Configuration/Radio/AvailRadio.ini`'s `comport=` line at whatever `ttyUSB<n>`
that symlink currently resolves to, every time the radio is plugged in.

This package installs both files under `/opt/minos2/Udev/` but does **not**
activate anything — the rule has to be filled in with your own radios'
`idVendor`/`idProduct`/`serial` before it means anything, and installing it
unedited would, at best, match nothing.

## Install

1. Connect one radio at a time and read its identity:
   ```sh
   udevadm info -a -n /dev/ttyUSB0
   ```
   take `ATTRS{idVendor}`, `ATTRS{idProduct}` and `ATTRS{serial}` from the
   first block that lists them (the CP210x/FTDI chip block, not the parent
   USB hub).

2. Copy the template and edit it for your radios:
   ```sh
   cp /opt/minos2/Udev/99-hamlib.rules.example ~/99-hamlib.rules
   ```
   - fill in the `idVendor`/`idProduct`/`serial` for each radio you have
   - replace `<user>` in every `RUN{program}` line with the Linux user
     account that runs Minos (the rule runs as root and has no other way to
     find that user's runtime tree)
   - if `MINOS2_RUNTIME` is set for that user, use that path instead of the
     default `~/minos2-runtime` shown in the template
   - drop any radio you don't have

3. Also edit the `%Radios` table near the top of
   `/opt/minos2/Udev/MinosSetRadioPorts` so it matches the radios and
   `/dev/<name>` symlinks defined in your rule (the comment above the table
   explains the format).

4. Install and reload:
   ```sh
   sudo cp ~/99-hamlib.rules /etc/udev/rules.d/
   sudo udevadm control --reload
   ```

5. Reconnect a radio and check:
   ```sh
   journalctl | grep MinosSetRadioPorts | tail
   grep -B2 comport= ~/minos2-runtime/Configuration/Radio/AvailRadio.ini
   ```

`MinosSetRadioPorts` needs `Config::IniFiles`, which is not one of this
package's dependencies since most installs never touch this feature:
`libconfig-inifiles-perl` (Debian/Ubuntu), `perl-Config-IniFiles`
(Fedora), or `perl-config-inifiles` (AUR) supplies it.

## Provenance

Vendored from [g0lgs/Minos-Config-Script](https://github.com/g0lgs/Minos-Config-Script)
(commit `a93570f`) by Stewart Wilkinson G0LGS, under the same
GPL-3.0-or-later terms as Minos itself. Relative to upstream, this copy
fixes:

- `ACTION="add"` (assignment, always true and an invalid operator for
  `ACTION`) on the second IC-9700 port, corrected to `ACTION=="add"` (match).
- Missing commas between key/value pairs on several rules, which
  `udevadm verify` reports as invalid.
- The `-cfg` path, updated from upstream's `~/runtime` to this project's
  `~/minos2-runtime` (see the top-level README's "Downloads" section).

`MinosSetRadioPorts` itself is carried over unmodified.
