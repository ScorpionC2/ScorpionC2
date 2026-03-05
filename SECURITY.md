# Security Policy

## Supported Versions

Security fixes are applied only to the latest development version of ScorpionC2.

Older versions may not receive security updates.

If possible, always use the latest commit from the `main` branch.

---

## Reporting a Vulnerability

If you discover a security vulnerability, **do not open a public GitHub issue**.

Instead, please report it privately.

You can report vulnerabilities by:

Opening a private GitHub security advisory.

Please include:

- Description of the vulnerability
- Steps to reproduce
- Affected version or commit hash
- Proof of concept (if possible)
- Impact assessment

We will investigate and respond as soon as possible.

---

## Responsible Disclosure

We ask that researchers follow responsible disclosure:

1. Do not publicly disclose the vulnerability before a fix is released
2. Give maintainers reasonable time to investigate and patch
3. Coordinate disclosure if the issue affects released versions

Security researchers will be credited when appropriate.

---

## Scope

This policy applies to vulnerabilities affecting:

- ScorpionC2 core
- Official modules
- Build system
- Network protocol implementations

Issues related to third-party dependencies should be reported to their respective maintainers.

---

## Security Best Practices

Contributors should follow secure coding practices:

- Avoid undefined behavior
- Validate external inputs
- Check all return values
- Avoid unsafe memory operations
- Ensure proper bounds checking

Pull requests introducing security risks may be rejected.