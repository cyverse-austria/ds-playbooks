# postgresql

Installes a PostgreSQL Database.

## Role Variables

| var-name            | default          | description                          |
| ------------------- | ---------------- | ------------------------------------ |
| postgresql_version  | 12               | postgres version to install          |
| postgresql_subnet   | 0.0.0.0/0        | subnet to reach the database         |
| postgresql_password | "changemeplease" | postgresPW should be set in pipeline |

## Example Playbook

```yaml
- hosts: all
  roles:
    - role: postgresql
      postgresql_version: 12
      postgresql_subnet: "0.0.0.0/0"
      postgresql_password: "changemeplease"
```
