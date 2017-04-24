#!/bin/bash
set -euo pipefail
cd "$(dirname "$0")"

sources=(*.c)
objects=()
target=wacom-plus
force=false
verbose=false

# Base flags
flags=(
	'-pipe'
)

cc_flags=(
	'-ansi'
	'-pedantic'
	'-I.'
	'-Wall'
	'-Wextra'
	'-Wshadow'
	'-Wmissing-prototypes'
	'-Wcast-qual'
	'-D_XOPEN_SOURCE=500'
	'-DGDK_VERSION_MIN_REQUIRED=GDK_VERSION_3_0'
	$(pkg-config --cflags 'gtk+-3.0' 'libwacom' | sed 's|-I|-isystem |g')
)

ld_flags=(
	$(pkg-config --libs 'gtk+-3.0' 'libwacom')
)

# Add env flags
if [[ -n "${CFLAGS-}" ]]; then
	cc_flags+=($CFLAGS)
fi

if [[ -n "${LDFLAGS-}" ]]; then
	ld_flags+=($LDFLAGS)
fi

# Functions
invoke() {
	local name="$1"
	local prog="$2"
	local dest="$3"
	shift 3

	local args=("$prog" "$@")

	if "$verbose"; then
		echo "${args[@]}"
	else
		echo "[$name] $dest"
	fi

	"${args[@]}"
}

check_sources() {
	local src="$1"
	local obj="$2"

	if "$force" || [[ ! -f "$obj" ]]; then
		return 1
	fi

	if [[ "$src" -nt "$obj" ]]; then
		return 1
	fi

	while read -r header; do
		if [[ "$header" -nt "$src" ]]; then
			return 1
		fi
	done < <(sed -rn 's|^#\s*include\s+"([^"]+)"|\1|p' "$src")
}

check_objects() {
	if "$force" || [[ ! -f "$target" ]]; then
		return 1
	fi

	for obj in "${objects[@]}"; do
		if [[ "$obj" -nt "$target" ]]; then
			return 1
		fi
	done
}

compile() {
	for src in "${sources[@]}"; do
		local obj="${src%.*}.o"

		# Append to objects array
		objects+=("$obj")

		# Check status
		if check_sources "$src" "$obj"; then
			continue
		fi

		# Compile the source file
		invoke 'CC' "${CC:-cc}" "$obj" "${flags[@]}" "${cc_flags[@]}" -c -o "$obj" "$src"
	done
}

link() {
	# Check status
	if check_objects; then
		return
	fi

	# Link the final binary
	invoke LD "${LD:-cc}" "$target" "${flags[@]}" "${ld_flags[@]}" -o "$target" "${objects[@]}"
}

clean() {
	rm -f *.o "$target"
}

# Parse arguments
for arg in "$@"; do
	case "$arg" in
		normal)
			flags+=('-O2')
			;;
		debug)
			flags+=('-g')
			;;
		release)
			flags+=('-O2' '-DNDEBUG' '-fstack-protector-strong')
			;;
		force)
			force=true
			flags+=('-O2')
			;;
		verbose)
			verbose=true
			;;
		clean)
			clean
			;;
		*)
			echo >&2 "Unknown argument: $arg"
			exit 1
			;;
	esac
done

# Run compilation
compile
link

# vim: set sw=4 ts=4 noet:
