using Microsoft.EntityFrameworkCore;
using ShopApp.Models;

namespace ShopApp.WebApi.Infrastructure
{
   public class ApplicationContext : DbContext
    {
        public DbSet<Product> Products { get; set; }

        public ApplicationContext(DbContextOptions<ApplicationContext> options) : base(options)
        {
        }

        protected override void OnModelCreating(ModelBuilder modelBuilder)
        {
            modelBuilder.Entity<Product>()
                .HasIndex(p => p.Id)
                .IsUnique();
        }
    }
}
